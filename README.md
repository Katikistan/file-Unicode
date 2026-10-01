# file(1) – Unicode

A simplified version of the Unix `file(1)` utility, written in C for a Computer Systems course.

Given a path, the program reports which kind of file it is:

| Output | Meaning |
|---|---|
| `empty` | The file has no content |
| `ASCII text` | Only printable ASCII and common control characters |
| `Unicode text, UTF-8 text` | Valid UTF-8 byte sequences |
| `ISO-8859 text` | ASCII plus ISO-8859 characters (160–254) |
| `data` | Anything else |

If the file can't be opened (missing, no read permission), it prints `<path>: cannot determine (<reason>)`.

## Build

```sh
make
```

## Usage

```sh
./file <path>
```

Example:

```sh
$ ./file hello.txt
hello.txt: ASCII text
```

## Testing

```sh
bash test.sh
```

The script builds the program, generates sample files in `test_files/`, and compares the output with the system `file` command. A few tests are expected to fail where the real `file(1)` recognises more types than this simplified version (e.g. "Non-ISO extended-ASCII text"), which the assignment asked us to classify as `data`.

## How it works

Checks run in order and the first match wins: **empty → ASCII → UTF-8 → ISO-8859 → data**. UTF-8 is checked before ISO-8859 because many byte sequences are valid in both, and valid UTF-8 is the less likely accident. UTF-8 detection follows the simplified encoding from the assignment (leading bytes `0xxxxxxx`, `110xxxxx`, `1110xxxx`, `11110xxx`, each followed by the right number of `10xxxxxx` bytes).

## Files

- `file.c` – the program
- `Makefile` – build and clean targets
- `test.sh` – test script
