# 📊 CSV Stats – C Command-Line Tool

A small **C project** that reads a CSV file and computes basic statistics for a chosen numeric column:

- Count
- Minimum
- Maximum
- Mean
- Standard deviation

My project demonstrates:

- File I/O (`fopen`, `fgets`)
- String parsing (`strtok_r`)
- Error handling
- Command-line argument parsing
- Basic numerical computation

---

## 🧱 Tech

- C (C11+ compatible)
- Standard library only (no external dependencies)
- Portable: should compile with `gcc` or `clang` on Linux/macOS/WSL

---

## 📂 Project Structure

```text
c_csv_stats/
├─ main.c        # Core logic
├─ Makefile      # Build commands
├─ sample.csv    # Example data
└─ README.md
```

---

## ▶️ How to Build & Run

### 1. Build

From the project folder:

```bash
make
```

This will compile `main.c` into an executable named `csv_stats`.

### 2. Run (with default arguments)

```bash
./csv_stats
```

- Uses `sample.csv`
- Uses column **1** by default

### 3. Run with custom file & column

```bash
./csv_stats data.csv 3
```

- Reads `data.csv`
- Uses the **3rd** (1-based) column as numeric

The program expects:

- First non-empty line = **header row** (skipped)
- Subsequent rows: comma-separated values
- Chosen column should be numeric

Non-numeric values and short rows are **skipped** with warnings.

---

## 📊 Example

Given `sample.csv`:

```csv
model,mpg,weight
CarA,21.0,2.620
CarB,22.8,2.320
CarC,18.7,3.440
CarD,24.4,3.190
CarE,17.3,3.440
```

Running:

```bash
./csv_stats sample.csv 2
```

Might print:

```text
CSV statistics for file 'sample.csv', column 2
--------------------------------------------------
Count:   5
Min:     17.300000
Max:     24.400000
Mean:    20.840000
Std dev: 3.036933
```
