# WAV Volume

A small C program that modifies the volume of a WAV audio file by multiplying its PCM samples by a user-defined factor.

## What does it do?

Digital audio is represented as a sequence of numerical values called *samples*. In uncompressed PCM audio, each sample represents the amplitude of the audio signal at a specific point in time.

This program reads a WAV file, copies its header, and multiplies each 16-bit sample by a given factor. This allows the user to increase or decrease the audio amplitude.

Instead of modifying the audio file as a whole, the program processes the individual samples that make up the signal.

## How it works

### 1. Command-line arguments

The program expects three arguments: the input file, the output file, and the volume factor.

```bash
./volume input.wav output.wav 2.0
```

The input file is opened in binary read mode (`rb`), while the output file is opened in binary write mode (`wb`). Binary mode ensures that the file contents are handled as raw bytes.

### 2. Copying the WAV header

```c
char header[44];

fread(header, 1, 44, input);
fwrite(header, 1, 44, output);
```

A typical PCM WAV file has a 44-byte header containing information such as the audio format, number of channels, sample rate, and data size.

Rather than reconstructing this information, the program copies the header directly into the output file.

However, not every WAV file has a 44-byte header. Additional chunks can exist, so a general-purpose WAV processor must parse the file structure instead of assuming a fixed header size.

### 3. Reading audio in blocks

```c
int16_t buffer[4096];
size_t samples_read;

while ((samples_read = fread(buffer, sizeof(int16_t), 4096, input)) > 0)
{
    // Process the samples
}
```

The program reads up to 4096 samples at a time.

Why not load the entire file into memory?

Because audio files can become large. Processing fixed-size blocks keeps memory usage predictable, regardless of the file's duration.

The return value of `fread()` tells us how many complete samples were actually read. Only those samples should be processed.

### 4. Modifying the volume

```c
int32_t sample = buffer[i];
sample = sample * factor;
buffer[i] = (int16_t) sample;
```

A signed 16-bit sample can represent values from `-32768` to `32767`.

The volume factor determines how the sample amplitude changes:

- `0.5`: reduces the amplitude by 50%.
- `1.0`: leaves the amplitude unchanged.
- `2.0`: doubles the amplitude.

The sample is temporarily stored in an `int32_t` because multiplication can produce a result outside the range of a 16-bit integer.

However, using a larger integer does not solve the overflow problem completely. If the result exceeds the range of `int16_t`, converting it back can produce an incorrect value. A more robust implementation should clamp the result to the valid range before converting it.

### 5. Writing the processed audio

After the samples have been modified, they are written to the output file. The process repeats until no more complete samples can be read.

The program processes the audio in a single pass and uses a fixed-size buffer, keeping memory consumption independent of the input file size.

## Compilation

Compile the program using GCC:

```bash
gcc volume.c -o volume
```

## Usage

Reduce the amplitude to 50%:

```bash
./volume input.wav output.wav 0.5
```

Increase the amplitude by 50%:

```bash
./volume input.wav output.wav 1.5
```

Double the amplitude:

```bash
./volume input.wav output.wav 2.0
```

Increasing the amplitude too much can cause clipping or distortion. The program currently does not prevent this.

## Current limitations

This project focuses on the fundamentals of processing binary audio data in C. It is not a complete WAV parser.

The current implementation has several limitations:

- Assumes a fixed 44-byte WAV header.
- Supports only 16-bit PCM samples.
- Does not validate whether the volume factor is a valid number.
- Does not prevent sample values from exceeding their valid range.
- Does not fully handle read and write errors.
- Does not close the input file if opening the output file fails.
- Does not check the return values of `fclose()`.

These limitations matter because modifying audio samples is only part of the problem. Reliable file processing also requires handling invalid input, unexpected file structures, and boundary conditions.

## What this project teaches

This project provides practical experience with several fundamental C concepts:

- Binary file I/O using `fread()` and `fwrite()`.
- Buffer-based processing and memory management.
- Integer types and their value ranges.
- Type conversions and numeric overflow.
- Command-line arguments.
- Error handling and resource management.

The algorithm itself is relatively simple. The interesting part is understanding how data is represented in memory, what assumptions a program makes about a file, and what happens when those assumptions are wrong.

That is where a small programming exercise starts becoming an engineering problem.