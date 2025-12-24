# Delay Audio Plugin

A professional-grade delay effects plugin for Audiokinetic Wwise, featuring multi-tap delays, chorus, and flanger effects with advanced DSP processing.

## Features

- **Multi-Tap Delay**: Up to 4 independent delay taps with individual feedback and filtering
- **Chorus/Flanger Effects**: Built-in modulation with LFO control
- **High-Performance DSP**: Native C++ implementation with SIMD optimization foundation
- **Comprehensive Testing**: Full unit test suite using Google Test
- **Cross-Platform**: Compatible with Windows and macOS

## Architecture

### Sound Engine Plugin (`SoundEnginePlugin/`)
Core DSP components written in native C++:

- **Delayline**: High-quality delay line with linear interpolation
- **CircularBuffer**: Lock-free circular buffer for audio buffering
- **Delay**: Multi-tap delay processor with feedback and filtering
- **ChorusFlanger**: Modulated delay effects
- **LFO**: Low-frequency oscillator for modulation effects
- **SimdOps**: SIMD-optimized operations for performance-critical paths

### Wwise Plugin (`WwisePlugin/`)
Wwise-specific integration layer:

- Plugin registration and lifecycle management
- Parameter definitions and UI binding
- Cross-platform GUI implementation

### Tests (`Tests/`)
Comprehensive unit test suite covering:

- Circular buffer operations and wraparound behavior
- Delay line interpolation and timing accuracy
- Helper functions (lerp, smoothing, range mapping)
- LFO phase progression and waveform generation

See [Tests/README.md](Tests/README.md) for testing documentation.

## Building

### Prerequisites

- Audiokinetic Wwise SDK (2023 or later)
- CMake 3.14+ (for tests)
- C++14 compatible compiler
- Premake (for plugin builds)
- `WWISESDK` environment variable pointing to Wwise SDK installation

### Building the Plugin

```bash
# Generate project files with Premake
premake5 vs2022  # For Visual Studio 2022
# or
premake5 gmake2  # For Make

# Build using generated project files
```

### Building and Running Tests

```bash
cd Tests
mkdir build && cd build
cmake ..
cmake --build .
./delay_tests
```

## Project Structure

```
Delay/
├── SoundEnginePlugin/       # Core DSP implementation
│   ├── Delay.cpp/h          # Main delay processor
│   ├── Delayline.cpp/h      # Single delay line
│   ├── CircularBuffer.cpp/h # Audio buffer management
│   ├── ChorusFlanger.cpp/h  # Modulation effects
│   ├── LFO.cpp/h            # Low-frequency oscillator
│   ├── DelayFX.cpp/h        # Wwise plugin wrapper
│   ├── DelayFXParams.cpp/h  # Parameter management
│   ├── Helpers.h            # Utility functions
│   └── SimdOps.h            # SIMD optimizations
├── WwisePlugin/             # Wwise integration layer
│   ├── DelayPlugin.cpp/h    # Plugin registration
│   └── Win32/               # Platform-specific GUI
├── Tests/                   # Unit test suite
├── FactoryAssets/           # Wwise factory presets
├── PremakePlugin.lua        # Build configuration
└── DelayConfig.h            # Plugin configuration

```

## Development

### Code Style

The project follows standard C++ conventions with:
- Consistent naming (PascalCase for types, camelCase for functions)
- RAII principles for resource management
- Modern C++14 features where appropriate

### Adding Features

1. Implement DSP logic in `SoundEnginePlugin/`
2. Add unit tests in `Tests/`
3. Update parameter definitions in `WwisePlugin/Delay.xml`
4. Extend GUI if needed in `WwisePlugin/Win32/`

### Testing

Always run the test suite before committing:

```bash
cd Tests/build
./delay_tests
```

## License

Copyright (c) 2023 Audiokinetic Inc.

This project uses portions of the Audiokinetic Wwise Technology released under dual licensing:
- Commercial License (for Wwise licensees)
- Apache License 2.0

See individual file headers for specific licensing information.

## Contributing

1. Write tests for new features
2. Ensure all tests pass
3. Follow the existing code style
4. Document public APIs

## Credits

Built with Audiokinetic Wwise SDK
