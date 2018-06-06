# XConf App

[![Greenkeeper badge](https://badges.greenkeeper.io/XConf/XConfApp.svg)](https://greenkeeper.io/)

## System Requirements

- [Node.js](https://nodejs.org/)
- [Yarn](https://yarnpkg.com/)
- Ruby
- macOS with Xcode
- JDK and Android development environment

## Setup

After the system requirements are settled, just run:

```bash
bin/setup
```

## Run

Start the Reason compiler and React Native packager by running:

```bash
yarn start
```

then, run `bin/run-ios` or `bin/run-android` to start the development iOS or Android app in the simulator.

## Test

Go through all the test suites by running:

```bash
bin/test
```

## Update Dependencies

After pulling a new version from the remote repository, you'll need to update the dependencies by running;

```bash
bin/update
```
