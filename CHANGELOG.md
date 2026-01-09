# Changelog

All notable changes to the Bitcoin Price Tracker project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Battery power support with LiPo and TP4056 charging module
- 3D-printable enclosure designs
- Historical price graph on display
- Multiple cryptocurrency support (ETH, BNB, etc.)
- Configurable price alerts with threshold settings
- Web interface for WiFi configuration
- MQTT integration for home automation
- E-ink display support for lower power consumption
- OTA (Over-The-Air) firmware updates

---

## [1.0.0] - 2025-01-08

### Added
- Initial release of Bitcoin Price Tracker
- Real-time Bitcoin price fetching from CoinDesk API
- 0.96" OLED display support (I2C, SSD1306)
- RGB LED visual indicators for price trends
  - Green: Price increased
  - Red: Price decreased
  - Blue: No change or initializing
- Audio alerts via passive buzzer
  - Success tone for price increases
  - Failure tone for price decreases
- Smart mute feature using photoresistor
  - Automatically disables sound in dark environments
  - Configurable light threshold
- Manual refresh button for instant price updates
- Non-blocking code architecture using millis()
- Price change display (shows dollar amount difference)
- WiFi connectivity with auto-reconnect
- Serial Monitor debugging output
- Comprehensive error handling

### Documentation
- Complete README.md with project overview and features
- Detailed WIRING.md with pin assignments and diagrams
- Bill of Materials (BOM.md) with component specifications
- SETUP.md guide for Arduino IDE and ESP32 configuration
- TROUBLESHOOTING.md for common issues and solutions
- Code comments and inline documentation
- MIT License

### Code Versions
- v1_basic: WiFi + OLED display only
- v2_led: Added RGB LED support (not included in this release)
- v3_sound: Added buzzer support (not included in this release)
- v4_final: Complete version with all features

### Hardware Support
- ESP32-WROOM-32 development boards
- DOIT ESP32 DEVKIT V1 (primary target)
- Compatible with most 30-pin ESP32 boards

### Libraries Required
- Adafruit SSD1306 (v2.x)
- Adafruit GFX Library (v1.x)
- ArduinoJson (v6.x)

---

## Version History Summary

### [1.0.0] - 2025-01-08
- Initial public release
- Full feature set implemented
- Complete documentation

---

## Notes

### Versioning Scheme
- **Major (X.0.0)**: Breaking changes, major new features
- **Minor (1.X.0)**: New features, backward compatible
- **Patch (1.0.X)**: Bug fixes, minor improvements

### Reporting Issues
Found a bug or have a feature request? 
Please open an issue at: https://github.com/nuniesmith/projects_btc_ticker/issues

### Contributing
Contributions are welcome! Please read CONTRIBUTING.md (coming soon) before submitting pull requests.

---

**Legend:**
- `Added` - New features
- `Changed` - Changes to existing functionality
- `Deprecated` - Features that will be removed
- `Removed` - Removed features
- `Fixed` - Bug fixes
- `Security` - Security improvements

---

*Last Updated: January 8, 2025*