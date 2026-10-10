#!/usr/bin/env bash
# Run KiCad's rule checks and exports on each project, using kicad-cli.
#
# CI runs this inside the official KiCad container (see
# .github/workflows/ci.yml). You can run it yourself if kicad-cli is on
# your PATH:
#
#     find hardware -name '*.kicad_pro' > kicad-projects.txt
#     bash tools/ci_kicad_checks.sh kicad-projects.txt ci-out
#
# For each project it:
#   - runs ERC on the root schematic and DRC on the board, and fails on
#     any violation of "error" severity (warnings are reported, not fatal)
#   - exports a schematic PDF and a BOM, for review and for the record
#
# It keeps going after a failure so that every report is produced, then
# exits non-zero if anything failed.
#
# Usage: ci_kicad_checks.sh <file listing .kicad_pro paths> <output folder>

set -u

list_file="${1:?usage: ci_kicad_checks.sh <project list file> <output folder>}"
out_root="${2:?usage: ci_kicad_checks.sh <project list file> <output folder>}"

failures=0
checked=0

kicad-cli version || { echo "kicad-cli not found"; exit 2; }

while IFS= read -r project; do
    [ -n "${project}" ] || continue
    base="${project%.kicad_pro}"
    name="$(basename "${base}")"
    out="${out_root}/${name}"
    mkdir -p "${out}"
    echo
    echo "=== ${project} ==="

    sch="${base}.kicad_sch"
    if [ -f "${sch}" ]; then
        checked=$((checked + 1))
        echo "--- ERC: ${sch}"
        if ! kicad-cli sch erc --severity-error --exit-code-violations \
                --output "${out}/${name}-erc.rpt" "${sch}"; then
            echo "::error file=${sch}::ERC reported errors. See ${name}-erc.rpt in the kicad-outputs artifact."
            failures=$((failures + 1))
        fi
        # A second, non-fatal pass so the report artifact lists warnings too.
        kicad-cli sch erc --severity-all \
            --output "${out}/${name}-erc-all.rpt" "${sch}" || true

        echo "--- Schematic PDF"
        kicad-cli sch export pdf --output "${out}/${name}-schematic.pdf" "${sch}" \
            || { echo "::error file=${sch}::Schematic PDF export failed."; failures=$((failures + 1)); }

        echo "--- BOM"
        kicad-cli sch export bom --output "${out}/${name}-bom.csv" "${sch}" \
            || { echo "::error file=${sch}::BOM export failed."; failures=$((failures + 1)); }
    else
        echo "No root schematic at ${sch}. The project file, root schematic and board must share one name."
        failures=$((failures + 1))
    fi

    pcb="${base}.kicad_pcb"
    if [ -f "${pcb}" ]; then
        checked=$((checked + 1))
        echo "--- DRC: ${pcb}"
        if ! kicad-cli pcb drc --severity-error --exit-code-violations \
                --schematic-parity \
                --output "${out}/${name}-drc.rpt" "${pcb}"; then
            echo "::error file=${pcb}::DRC reported errors. See ${name}-drc.rpt in the kicad-outputs artifact."
            failures=$((failures + 1))
        fi
        kicad-cli pcb drc --severity-all --schematic-parity \
            --output "${out}/${name}-drc-all.rpt" "${pcb}" || true
    else
        echo "No board at ${pcb} yet. DRC skipped for this project."
    fi
done < "${list_file}"

echo
echo "Checked ${checked} file(s). ${failures} failure(s)."
[ "${failures}" -eq 0 ]
