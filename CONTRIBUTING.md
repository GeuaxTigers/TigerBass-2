# Contributing to TigerBass 2

How we work in this repository. If you have never used Git, read this top to bottom once. After that, the [everyday loop](#the-everyday-loop) is the only part you need regularly.

## Contents

- [The idea in one minute](#the-idea-in-one-minute)
- [First-time setup](#first-time-setup)
- [The everyday loop](#the-everyday-loop)
- [KiCad files](#kicad-files)
- [Reviewing a pull request](#reviewing-a-pull-request)
- [Fabrication releases](#fabrication-releases)
- [What goes in your design notebook](#what-goes-in-your-design-notebook)
- [When something goes wrong](#when-something-goes-wrong)

## The idea in one minute

- The **repository** is the project folder plus its whole history. GitHub holds the shared copy. You work on your own copy on your computer.
- **`main`** is the version everyone trusts. We could order boards or flash firmware from it at any time.
- You never change `main` directly. You make a **branch**, which is your own line of work, and change things there.
- A **commit** is a saved step on your branch with a message saying what you did.
- When your work is ready you **push** the branch to GitHub and open a **pull request**: a request to merge your branch into `main`.
- A teammate **reviews** it. Automatic checks run on it. When both are happy, it is **merged**.

Nothing you do on a branch can damage `main` or anyone else's work. The worst case is that you delete the branch and start again.

## First-time setup

Do this once per computer.

### Option A: GitHub Desktop

Pick this if you have not used a command line much.

1. Install [GitHub Desktop](https://desktop.github.com/) and sign in with your GitHub account.
2. File → Clone repository → choose `TigerBass-2` → pick a folder on your computer → Clone.

Put the folder somewhere that is **not** synced by OneDrive, Dropbox, iCloud or Google Drive. Sync services and Git both try to manage the same files and corrupt each other's work.

### Option B: command line

1. Install [Git](https://git-scm.com/downloads).
2. Tell Git who you are. Use the email on your GitHub account.

   ```bash
   git config --global user.name "Your Name"
   git config --global user.email "you@example.com"
   ```

3. Clone the repository.

   ```bash
   git clone https://github.com/radharama351/TigerBass-2.git
   cd TigerBass-2
   ```

### Then, for everyone

Install the tools listed under "Getting set up" in the [README](README.md#getting-set-up), at the versions given there.

## The everyday loop

### 1. Get the latest `main`

Always start here. Starting from an old copy of `main` is how most conflicts happen.

| GitHub Desktop | Command line |
| --- | --- |
| Current Branch → `main`, then Fetch origin, then Pull origin | `git switch main` then `git pull` |

### 2. Make a branch

Name it with the kind of work and a few words about it.

| Prefix | For |
| --- | --- |
| `hw/` | Schematics, layout, libraries, simulations |
| `fw/` | Firmware |
| `mech/` | Mechanical |
| `docs/` | Documentation, analysis, CI and repository tooling |

Examples: `hw/adc-front-end`, `fw/note-map`, `mech/battery-mount`, `docs/latency-budget`.

| GitHub Desktop | Command line |
| --- | --- |
| Current Branch → New Branch | `git switch -c hw/adc-front-end` |

### 3. Do the work, and commit as you go

Commit whenever you reach a point you might want to return to. Small commits are easier to review and easier to undo.

Write the message as an instruction, with a prefix for the area:

```text
hw: add input buffer to pickup channel 1
fw: debounce fret lines over three scans
docs: record ADC decision
```

| GitHub Desktop | Command line |
| --- | --- |
| Tick the files to include, type a summary, Commit | `git add <files>` then `git commit -m "hw: add input buffer to pickup channel 1"` |

Before each commit, look at the list of changed files. If it includes a file you did not mean to change, find out why before committing. In GitHub Desktop, right-click the file → Discard changes. On the command line, `git restore <file>`.

### 4. Push and open a pull request

| GitHub Desktop | Command line |
| --- | --- |
| Publish branch, then Create Pull Request | `git push -u origin hw/adc-front-end`, then open the link Git prints |

Fill in the template. The two parts reviewers need most are **what changed** in plain words and **how you checked it**.

If the work is not finished but you want feedback, open it as a **draft** pull request.

### 5. Respond to review

Reviewers leave comments. Make the changes on the same branch, commit, and push again. The pull request updates itself. Reply to each comment so the reviewer knows it was handled.

### 6. Merge

When the pull request has an approval and the checks are green, click **Squash and merge**, then delete the branch. Go back to step 1 for the next piece of work.

## KiCad files

This is the part of the project where Git needs the most care.

### Why they are different

Git merges two people's changes to a text file by combining the lines each of them changed. That works for C code and Markdown.

KiCad schematic and board files are text, but one visible change, such as moving a component, rewrites coordinates and identifiers across many lines. If two people edit the same file, Git either reports a conflict nobody can resolve by reading it, or worse, produces a merged file that opens but has a wire or track in the wrong place. Nobody would see that in a diff.

### The rule

**One person edits a given KiCad file at a time.**

A "file" here is one `.kicad_sch` sheet or one `.kicad_pcb` board. Splitting the schematic into hierarchical sheets, one per function, is what lets several people work on the same board at once: one person on the power sheet, another on the ADC sheet.

There is only one board file per board. Layout is one person at a time.

### How to claim a file

1. Find or open the issue for the work.
2. Comment: "Editing `hardware/controller-board/adc.kicad_sch` on branch `hw/adc-front-end`."
3. Pull the latest `main` and branch from it.
4. Make the change. Keep it small. Aim to open the pull request the same day.
5. When it merges, comment that the file is free.

If someone has claimed a file you need, ask them. Do not start a parallel edit.

### Before every KiCad commit

1. Save in KiCad, then **close KiCad**. Closing removes its lock file and finishes writing the project file.
2. Run **ERC** in the schematic editor (Inspect → Electrical Rules Checker) and **DRC** in the board editor (Inspect → Design Rules Checker).
3. If you changed the schematic, update the board from it (Tools → Update PCB from Schematic) so the two agree.
4. Look at the changed-files list. It should contain only the files you meant to edit. A `.kicad_prl`, a `-backups` folder or a `.lck` file in the list means something is wrong with `.gitignore`. Tell the team.
5. **Commit with GitHub Desktop or command-line Git, never by uploading files on the GitHub website.** Desktop and Git skip the files listed in `.gitignore`. The website's Add file → Upload files does not, so a dragged-in project folder brings its backups and `.kicad_prl` with it.

### If leftovers get pushed anyway

A cleanup bot catches them. Whenever anyone pushes to a branch other than `main`, the [clean-editor-leftovers](.github/workflows/clean-editor-leftovers.yml) workflow looks for KiCad backups, `.kicad_prl`, lock and autosave files, and build output. If it finds any, it removes them from Git, pushes a commit named `ci: remove editor leftovers from Git` to your branch, and re-runs the checks.

What that means for you:

- **Pull before you push again.** The bot's commit is on GitHub but not on your computer. In GitHub Desktop, click **Pull origin**. On the command line, `git pull`.
- **When you pull, Git may delete those leftover files from your folder,** because the bot told Git to stop tracking them. That is harmless: KiCad recreates `.kicad_prl` and new backups the next time you open the project.
- **If the bot cannot fix it,** the "Repository checks" job still fails the pull request and lists the files, so they can never reach `main`.

### If two people edited the same file anyway

1. Stop. Do not try to resolve the conflict by editing the file's text.
2. Decide together whose version is kept, usually the one with more work in it.
3. Keep that version whole.
4. The other person opens the kept version in KiCad and redoes their change by hand.

On the command line, to keep one side of a conflicted file whole:

```bash
git checkout --theirs path/to/file.kicad_sch    # keep the version from the branch being merged in
git checkout --ours   path/to/file.kicad_sch    # keep the version on your current branch
git add path/to/file.kicad_sch
```

If you are not sure which is which, ask before running either.

### One KiCad version

Everyone installs the same KiCad major version, the one listed in the README. A newer KiCad saves files in a format an older one cannot open. One person upgrading and saving locks everyone else out until they upgrade too. Upgrading is a team decision and happens for everyone on the same day.

## Reviewing a pull request

Every pull request needs one approval from a teammate who did not write it. Reviewing is how a second person comes to understand each part of the design, which matters when the first person is out the week of a demo.

### Any pull request

1. Read the description. Do you understand what changed and why? If not, ask. That is a valid review comment.
2. Check the "How it was checked" section. Is there evidence, or only a claim?
3. Look at the Files changed tab. Is anything in there that should not be?
4. Check that the automatic checks are green.

### Firmware

Read the diff. Then get the branch and run the tests yourself:

```bash
git fetch
git switch <branch-name>
cmake -S firmware -B build/host -DTB2_BUILD_TESTS=ON
cmake --build build/host
ctest --test-dir build/host --output-on-failure
```

Ask: is there a test for the new behaviour? Does anything in the scan path wait?

### Hardware

GitHub cannot show a useful diff of a schematic or board, so review it in KiCad.

1. Get the branch (GitHub Desktop: Current Branch → pick it. Command line: `git fetch` then `git switch <branch-name>`).
2. Open the project in KiCad.
3. Run ERC and DRC yourself. Compare with what the pull request says.
4. Compare against `main`. The CI run on the pull request uploads a schematic PDF as an artifact. Download it, and the one from the latest `main` run, and put them side by side.
5. For each new or changed part, open its datasheet and check: pinout against the symbol, footprint against the recommended land pattern, supply and signal voltages within the absolute maximum ratings, decoupling as the datasheet recommends.
6. Switch back to `main` when you are done. Do not save anything while reviewing.

### Mechanical

Open the STEP file and the drawing. Check the dimensions that are shared with the electronics, listed in [`mechanical/README.md`](mechanical/README.md), against the current board outline.

### Pull requests from Copilot

Review them the same way, with extra attention to one thing: any number, part number or pin assignment that does not cite a source. Copilot is told to mark what it does not know as `TBD(team)`. If it stated something as fact without a source, ask for the source or change it to `TBD(team)`.

## Fabrication releases

We never order a board from whatever happens to be on someone's laptop. An order comes from a **tag**: a named, permanent pointer to one commit on `main`.

1. Everything for the revision is merged to `main`, and ERC and DRC are clean or each remaining violation is explained in writing.
2. A second person does a full review using the hardware steps above, plus a paper check: print the board at 1:1 and place the real connectors and largest parts on the printout.
3. Tag the commit. Board name, then revision letter.

   ```bash
   git switch main
   git pull
   git tag -a ctrl-revA -m "Controller board revision A, as ordered"
   git push origin ctrl-revA
   ```

4. From that exact commit, generate the fabrication outputs in KiCad: gerbers, drill files, the position file, the BOM, a schematic PDF, and a STEP export for the MEs.
5. On GitHub: Releases → Draft a new release → choose the tag → attach the outputs as one zip, plus the schematic PDF separately → write what is in this revision and what is known to be wrong with it → Publish.
6. Order from the files attached to the release.
7. Write the tag name on the order and in your notebook.

When the boards arrive and something needs a bodge wire, open an issue labelled with the revision. Those issues are the to-do list for the next revision.

Tag names: `ctrl-revA`, `ctrl-revB` for the controller board; `fret-revA` for the fret board; `fw-v0.1.0` for firmware that was demonstrated or handed over.

## What goes in your design notebook

A merged pull request is most of a notebook entry already.

| Notebook entry needs | Take it from |
| --- | --- |
| What you did | The pull request's "What changed" |
| Why | The pull request's "Why", and the decision record it links to |
| Evidence | "How it was checked": ERC and DRC results, test output, scope captures, simulation plots |
| Your own contribution | The commits under your name. On GitHub, the Commits tab of the pull request |
| Date | The merge date |
| Category | Planning, research, ideation, analysis, documentation, manufacturing, testing or safety, whichever the work was |

Reviewing counts too. A review where you checked a footprint against a datasheet and found a problem is analysis you did, with a link to prove it.

Write the pull request description properly at the time and the notebook entry takes five minutes. Check the Capstone Design Program's own rules for what a notebook entry must contain, and for its policy on AI-assisted work.

## When something goes wrong

| Situation | What to do |
| --- | --- |
| "I committed to `main` by mistake" and have not pushed | Make a branch right where you are (`git switch -c hw/my-work`). Your commit comes with you. Ask someone to help reset your local `main` |
| "I pushed something I should not have", such as a password or a huge file | Tell the team straight away. Do not try to hide it with another commit: the file stays in history until it is removed properly |
| "Git says there is a conflict" in a text file | Open the file, find the `<<<<<<<` markers, keep the right lines, delete the markers, commit. Ask if unsure |
| "Git says there is a conflict" in a KiCad file | See [If two people edited the same file anyway](#if-two-people-edited-the-same-file-anyway) |
| "My branch is behind `main`" | On your branch: `git fetch`, then `git merge origin/main`. In GitHub Desktop: Branch → Update from main |
| "I have changes I do not want" | GitHub Desktop: right-click the file → Discard changes. Command line: `git restore <file>`. This cannot be undone |
| "The checks failed on my pull request" | Click Details next to the red check and read the last lines of the log. The repository checks can be run locally: see [`tools/README.md`](tools/README.md) |
| "I have no idea what state I am in" | Run `git status` and read it, or paste it to the team chat. Do not start deleting things |
