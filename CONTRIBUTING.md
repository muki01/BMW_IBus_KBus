# Contributing to BMW I-Bus / K-Bus Firmware

Thank you for taking the time to contribute! Every vehicle test, bug report, fix and idea makes this project better for everyone who drives and tinkers with a classic BMW.

By participating, you agree to follow the [Code of Conduct](CODE_OF_CONDUCT.md).

## Ways to Contribute

| | |
|---|---|
| 🚗 **Report a tested vehicle** | Tell us which cars work (or don't). Use the **Vehicle report** issue template. |
| 🐛 **Report a bug** | Use the **Bug report** template and include the serial log whenever possible. |
| 💡 **Suggest a feature** | Use the **Feature request** template. |
| 📡 **Share messages** | Verified I-Bus / K-Bus messages for other chassis (E38, E39, E53, E83, E85). |
| 🔧 **Submit code** | Fixes, new functions, new buttons for the web interface, board support. |
| 📝 **Improve the docs** | Clearer instructions, wiring photos and connection points for other models. |

## Reporting Bugs

Before opening an issue, please search the [existing issues](https://github.com/muki01/BMW_IBus_KBus/issues). A good report includes:

- The firmware (`E46_KBus_ESP32`, `E46_KBus_Code` or `Basic_Code`) and the commit you are using
- The board (e.g. ESP32 DevKit, Arduino Nano) and the Arduino core version
- The interface circuit (TH3122.4, ELMOS 10026B, MCP2025, optocouplers)
- The vehicle: model, year and equipment that matters (for example xenon lights)
- **The debug output.** The `Good Message -> …` and `TRANSMITING CODE: …` lines are the most useful information.

## Development Workflow

1. **Fork** the repository and create a branch from `main`:
   ```bash
   git checkout -b feature/my-improvement
   ```
2. Make your changes, keeping them **focused**: one fix or feature per pull request.
3. **Test in a car** when your change touches the messages or the bus communication, and say in the pull request which vehicle you tested on.
4. Make sure all three sketches still **compile** for the boards they support. The same check runs automatically on every pull request.
5. Commit with a clear message, e.g. `Add rear fog light message`.
6. Push and open a **pull request** against `main`, filling in the template.

## Coding Guidelines

- Follow the existing style of the file you are editing: naming, indentation and comment density.
- Store messages **without** their checksum and without a fixed array size, as in `E46_Codes.h`. The library appends the checksum.
- Keep `E46_Codes.h` identical in `Codes/E46_KBus_Code` and `Codes/E46_KBus_ESP32`.
- Wrap constant debug strings in `F()` to save RAM on AVR boards.
- New buttons for the web interface go in `Commands.h`; the page builds itself from that list.
- Never commit a Wi-Fi password, and keep `WIFI_PASSWORD` in `Config.h` empty.

## Bus Communication Changes

The code that receives and transmits on the bus lives in the **[BMW IBus KBus library](https://github.com/muki01/BMW_IBus_KBus_Library)**. Please open pull requests for it there.

## License

By contributing, you agree that your contributions will be licensed under the [MIT License](LICENSE).
