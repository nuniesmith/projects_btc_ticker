# Contributing to Bitcoin Price Tracker

Thank you for your interest in contributing to the Bitcoin Price Tracker project! This document provides guidelines for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Enhancements](#suggesting-enhancements)

## Code of Conduct

This project adheres to a code of conduct that all contributors are expected to follow:

- **Be respectful**: Treat everyone with respect and kindness
- **Be constructive**: Provide helpful feedback and suggestions
- **Be collaborative**: Work together to improve the project
- **Be patient**: Remember that contributors have varying skill levels

## How Can I Contribute?

### 1. Share Your Build

The most valuable contribution is sharing photos and experiences:

- **Photos**: Clear images of your completed build
- **Videos**: Demonstration of features and assembly process
- **Build logs**: Document your assembly process
- **Tips & tricks**: Share what worked well for you

### 2. Improve Documentation

Documentation is always appreciated:

- Fix typos or clarify instructions
- Add missing information
- Translate documentation to other languages
- Create tutorial videos or blog posts

### 3. Report Bugs

Found a problem? Help us fix it:

- Check if the issue already exists
- Provide detailed steps to reproduce
- Include Serial Monitor output
- Share photos of your wiring if relevant

### 4. Suggest Features

Have an idea for improvement?

- Describe the feature clearly
- Explain why it would be useful
- Consider backward compatibility
- Be open to discussion

### 5. Submit Code

Contribute code improvements:

- Bug fixes
- New features
- Performance improvements
- Code refactoring

## Getting Started

### Fork and Clone

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/projects_btc_ticker.git
   cd projects_btc_ticker
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/nuniesmith/projects_btc_ticker.git
   ```

### Set Up Development Environment

1. Install Arduino IDE (2.x recommended)
2. Install ESP32 board support
3. Install required libraries:
   - Adafruit SSD1306
   - Adafruit GFX Library
   - ArduinoJson
4. Connect your ESP32 hardware for testing

## Development Process

### Branch Strategy

- `main` - Stable, tested code
- `develop` - Integration branch for features
- `feature/feature-name` - New features
- `bugfix/issue-name` - Bug fixes
- `docs/topic` - Documentation updates

### Creating a Branch

```bash
# Update your local repository
git checkout main
git pull upstream main

# Create a new branch
git checkout -b feature/your-feature-name
```

### Making Changes

1. Make your changes in small, logical commits
2. Test thoroughly on real hardware
3. Update documentation if needed
4. Add comments to explain complex code

### Testing Your Changes

Before submitting, verify:

- ✅ Code compiles without errors
- ✅ Tested on real ESP32 hardware
- ✅ All features work as expected
- ✅ Serial Monitor shows no errors
- ✅ WiFi connects successfully
- ✅ API requests work properly
- ✅ Display shows correct information
- ✅ LEDs, buzzer, sensors respond correctly

## Coding Standards

### Arduino/C++ Style

Follow these conventions for consistency:

#### Naming Conventions

```cpp
// Constants: UPPER_CASE
#define PIN_LED 13
const int LIGHT_THRESHOLD = 1000;

// Variables: camelCase
int currentPrice = 0;
float lastPrice = 0.0;

// Functions: camelCase
void updateDisplay() {
  // function code
}

// Classes: PascalCase (if used)
class PriceTracker {
  // class code
};
```

#### Indentation and Formatting

- Use **2 spaces** for indentation (not tabs)
- Opening brace on same line for functions
- Add blank lines between logical sections
- Maximum line length: 100 characters (recommended)

```cpp
void goodExample() {
  if (condition) {
    doSomething();
  } else {
    doSomethingElse();
  }
}
```

#### Comments

```cpp
// Single-line comments for brief explanations

/*
 * Multi-line comments for longer descriptions
 * or function documentation
 */

/**
 * Function description
 * Parameters: description
 * Returns: description
 */
void myFunction() {
  // Implementation
}
```

#### Pin Definitions

Keep all pin definitions together at the top:

```cpp
// --- PIN DEFINITIONS ---
#define PIN_RED     15
#define PIN_GREEN   2
#define PIN_BLUE    4
#define PIN_BUZZER  13
// ... etc
```

#### Magic Numbers

Avoid magic numbers - use named constants:

```cpp
// Bad
if (analogRead(34) > 1000) {

// Good
const int LIGHT_THRESHOLD = 1000;
if (analogRead(PIN_LDR) > LIGHT_THRESHOLD) {
```

### Documentation Standards

#### Code Comments

- Comment the "why", not the "what"
- Explain complex algorithms
- Document assumptions
- Note hardware dependencies

```cpp
// Bad comment (obvious)
i++; // Increment i

// Good comment (explains why)
delay(50); // Debounce delay to prevent multiple button triggers
```

#### README Updates

When adding features, update:
- Feature list
- Pin assignments (if changed)
- Wiring instructions
- Setup requirements

## Commit Guidelines

### Commit Messages

Use clear, descriptive commit messages:

#### Format

```
<type>: <short description>

<optional detailed description>

<optional footer>
```

#### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style/formatting (no logic change)
- `refactor`: Code refactoring
- `test`: Adding/updating tests
- `chore`: Maintenance tasks

#### Examples

```bash
feat: Add battery voltage monitoring

- Read battery voltage from ADC
- Display low battery warning
- Auto-sleep when voltage too low

Closes #42
```

```bash
fix: Correct RGB LED color mapping

Red and blue pins were swapped in pin definitions.
Updated to match standard pinout.

Fixes #38
```

```bash
docs: Add photoresistor calibration guide

Explain how to adjust LIGHT_THRESHOLD based on
room lighting conditions.
```

### Commit Best Practices

- **One logical change per commit**
- **Test before committing**
- **Write clear messages**
- **Reference issues** when relevant

## Pull Request Process

### Before Submitting

1. ✅ Sync with upstream main
2. ✅ Test all changes on hardware
3. ✅ Update documentation
4. ✅ Follow coding standards
5. ✅ Write clear commit messages

### Submitting a Pull Request

1. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create Pull Request** on GitHub:
   - Go to the original repository
   - Click "New Pull Request"
   - Select your branch
   - Fill out the template

3. **PR Title**: Use same format as commit messages
   ```
   feat: Add multi-currency support
   ```

4. **PR Description**: Include:
   - What changed
   - Why it changed
   - How to test it
   - Screenshots/videos (if UI change)
   - Related issue numbers

### PR Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring

## Testing
- [ ] Tested on ESP32 hardware
- [ ] Code compiles without errors
- [ ] All features work as expected

## Checklist
- [ ] Code follows project style guidelines
- [ ] Documentation updated
- [ ] No new warnings introduced
- [ ] Tested on real hardware

## Related Issues
Closes #issue_number
```

### Review Process

- Maintainers will review your PR
- Address feedback by pushing new commits
- Be patient - reviews may take a few days
- Be open to suggestions and changes

## Reporting Bugs

### Before Reporting

1. Check existing issues
2. Test with latest version
3. Verify wiring is correct
4. Try basic troubleshooting

### Bug Report Template

```markdown
**Describe the Bug**
Clear description of what's wrong

**To Reproduce**
Steps to reproduce:
1. Upload code
2. Connect to WiFi
3. See error

**Expected Behavior**
What should happen

**Actual Behavior**
What actually happens

**Hardware**
- ESP32 Board: DOIT ESP32 DEVKIT V1
- OLED Display: 0.96" I2C SSD1306
- Other components: [list]

**Software**
- Arduino IDE version: 2.2.1
- ESP32 Board Package: 2.0.14
- Library versions: [list]

**Serial Monitor Output**
```
Paste relevant output here
```

**Photos**
Attach photos of wiring if relevant

**Additional Context**
Any other information
```

## Suggesting Enhancements

### Feature Request Template

```markdown
**Feature Description**
Clear description of the feature

**Use Case**
Why is this feature useful?
Who would benefit?

**Proposed Implementation**
Ideas on how to implement (optional)

**Alternatives Considered**
Other approaches you thought about

**Additional Context**
Mockups, examples, references
```

## Recognition

Contributors will be recognized in:
- README.md acknowledgments
- Release notes
- Git commit history

Thank you for helping make this project better! 🚀

---

## Questions?

- **General questions**: Open a discussion on GitHub
- **Bug reports**: Open an issue
- **Security issues**: Email maintainers privately

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

*Last updated: January 2025*