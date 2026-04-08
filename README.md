# SORInterviewPrep
SOR Interview Prep low latency

## False sharing detection demo

This repo now includes a small benchmark in `src/main.cpp` that runs two cases:

- `UnpaddedCounter`: counters are adjacent in memory (likely false sharing)
- `PaddedCounter`: each counter is cache-line padded (false sharing reduced)

### Build and run

```bash
cmake -S . -B build
cmake --build build -j
./build/HelloWorld
```

Expected result: the `Unpadded` case is usually slower than `Padded`.

### Detect with perf c2c (Linux)

`perf c2c` can highlight cache line contention with HITM (hit-modified) events.

1. Record memory access samples:

```bash
cd build
perf c2c record -- ./HelloWorld
```

2. Report and inspect hot lines:

```bash
perf c2c report --stdio | less
```

Look for:

- high `Load Local HITM` / `Load Remote HITM`
- hot lines attributed to `run_benchmark<UnpaddedCounter>`

Those are strong indicators of false sharing.

### If perf access is blocked in container/host

Some systems restrict perf for non-root users. If needed, lower paranoia temporarily:

```bash
sudo sysctl -w kernel.perf_event_paranoid=1
```

If `perf c2c` is unavailable on your CPU/kernel, use this fallback signal:

```bash
perf stat -d ./build/HelloWorld
```

