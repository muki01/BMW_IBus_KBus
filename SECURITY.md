# Security Policy

The ESP32 firmware in this repository can unlock doors, open windows and switch lights over Wi-Fi. Security reports are therefore taken seriously.

## Supported Versions

Security fixes are applied to the latest code on the `main` branch.

## Reporting a Vulnerability

If you find a security issue — for example in the web server, the settings handling or the Wi-Fi access point — **please do not open a public issue**.

Instead, email **muksin.muksin04@gmail.com** with:

- A description of the issue and its potential impact
- Steps to reproduce (board, firmware, configuration)
- A suggested fix, if you have one

You will receive a response as soon as possible, and credit in the release notes if you wish.

## Hardening Tips for Users

- Choose a **long, unique Wi-Fi password** in `Config.h`. Anyone who can join the network can control the car. The firmware has no default password for this reason.
- The web interface has no login of its own; the Wi-Fi password is the only barrier.
- The access point is only active while the ESP32 is powered, that is, while the bus is awake or shortly after you used the web interface.
- Remove buttons you do not need from `Commands.h`, for example the lock and window commands.
- Never publish your `Config.h` with the password filled in.
