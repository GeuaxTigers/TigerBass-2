#!/usr/bin/env python3
"""TEMPORARY: summarise a KiCad netlist and ERC report for the restructure PR.

Usage: tmp_verify_summary.py <netlist.xml> <erc.json>

Prints Markdown with:
  * a SHA-256 of the circuit's connectivity, keyed by each symbol's UUID
    rather than its reference, so re-annotation and moving a symbol to
    another sheet do not change it but any change to what connects to
    what does;
  * a SHA-256 of the component list (UUID, value, footprint, library part);
  * ERC violation counts by type and severity;
  * the reference of every symbol, so old and new references can be matched.

Removed before the restructure pull request merges.
"""

import collections
import hashlib
import json
import sys
import xml.etree.ElementTree as ET


def main(net_path, erc_path):
    root = ET.parse(net_path).getroot()
    ref_to_id = {}
    comps = []
    sheets = {}
    for comp in root.iter("comp"):
        ref = comp.get("ref")
        ts = comp.find("tstamps")
        uuids = " ".join(sorted((ts.text or "").split())) if ts is not None else ref
        ref_to_id[ref] = uuids
        lib = comp.find("libsource")
        sp = comp.find("sheetpath")
        sheet = sp.get("names") if sp is not None else "?"
        sheets.setdefault(sheet, []).append(ref)
        comps.append((uuids,
                      (comp.findtext("value") or ""),
                      (comp.findtext("footprint") or ""),
                      f"{lib.get('lib')}:{lib.get('part')}" if lib is not None else ""))
    nets = []
    for net in root.iter("net"):
        nodes = sorted(f"{ref_to_id.get(n.get('ref'), n.get('ref'))}#{n.get('pin')}"
                       for n in net.iter("node"))
        if nodes:
            nets.append("|".join(nodes))
    nets.sort()
    conn_hash = hashlib.sha256("\n".join(nets).encode()).hexdigest()
    comp_hash = hashlib.sha256("\n".join("\t".join(c) for c in sorted(comps)).encode()).hexdigest()

    erc = json.load(open(erc_path))
    counts = collections.Counter()
    for sheet in erc.get("sheets", []):
        for v in sheet.get("violations", []):
            counts[(v.get("severity"), v.get("type"))] += 1

    out = []
    out.append(f"**KiCad:** {erc.get('kicad_version', '?')}")
    out.append("")
    out.append(f"- Components: {len(comps)}")
    out.append(f"- Nets with at least one pin: {len(nets)}")
    out.append(f"- Connectivity hash: `{conn_hash}`")
    out.append(f"- Component hash: `{comp_hash}`")
    out.append("")
    out.append("| ERC severity | Type | Count |")
    out.append("| --- | --- | --- |")
    for (sev, typ), n in sorted(counts.items()):
        out.append(f"| {sev} | {typ} | {n} |")
    if not counts:
        out.append("| none | | 0 |")
    out.append("")
    out.append("<details><summary>References by sheet</summary>\n")
    for sheet, refs in sorted(sheets.items()):
        out.append(f"- `{sheet}`: " + ", ".join(sorted(refs)))
    out.append("\n</details>\n")
    out.append("<details><summary>Reference to symbol UUID</summary>\n")
    out.append("```text")
    for ref, uid in sorted(ref_to_id.items()):
        out.append(f"{ref}\t{uid}")
    out.append("```\n</details>")
    print("\n".join(out))


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
