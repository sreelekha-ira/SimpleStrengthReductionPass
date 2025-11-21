# SimpleStrengthReductionPass

Small LLVM Function Pass (new pass manager) that replaces integer multiplication by 2 with a left shift (`shl`) — a classic strength reduction example.

## Files included
- `passes/StrengthReduction.cpp` — the pass implementation (new pass manager plugin)
- `test/input.ll` — sample IR to test the pass
- `CMakeLists.txt` — build script using LLVM CMake config
- `.github/workflows/ci.yml` — optional CI to build and run the pass on Ubuntu
- `.vscode/tasks.json` — optional VS Code tasks (configure + build)
- `NVIDIA 6M Spring - LLVM Compiler Intern JD.pdf` — your uploaded job description (for reference)
- `.gitignore` — standard ignores for build artifacts

## Quick build & run (WSL / Ubuntu)

```bash
sudo apt update
sudo apt install -y build-essential cmake git clang llvm lld
mkdir build && cd build
cmake ..
cmake --build . -- -j$(nproc)
```

Run the pass on the example IR:

```bash
opt -load-pass-plugin ./SimpleStrengthReductionPass.so -passes=strength-reduction ../test/input.ll -S -o out.ll
cat out.ll
```

You should see `shl` replacing `mul` in `out.ll`.

## GitHub push (example)

```bash
git init
git add .
git commit -m "Initial: Strength reduction LLVM pass"
git branch -M main
git remote add origin https://github.com/<your-username>/SimpleStrengthReductionPass.git
git push -u origin main
```

