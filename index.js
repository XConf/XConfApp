import { AppRegistry } from 'react-native'
import { app } from './src/app.bs'

import './initializers/console'

if (__DEV__) {
  require('./initializers/yello-box')
}

AppRegistry.registerComponent('XConf', () => app)
