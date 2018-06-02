# XConf App

[![Greenkeeper badge](https://badges.greenkeeper.io/XConf/XConfApp.svg)](https://greenkeeper.io/)

## System Requirements

- [Node.js](https://nodejs.org/)
- [Yarn](https://yarnpkg.com/)
- macOS with Xcode
- JDK and Android development environment

## Setup

After the system requirements are settled, just run:

```bash
bin/setup
```

## Run

Start the Reason compiler by running:

```bash
yarn run watch
```

and Metro Bundler:

```bash
yarn start
```

then, run `react-native run-ios` or `react-native run-android`.

## Update Dependencies

After pulling a new version from the remote repository, you'll need to update the dependencies by running;

```bash
bin/update
```
