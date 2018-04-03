/* eslint-disable quotes, max-len */
import { YellowBox } from 'react-native'

YellowBox.ignoreWarnings([
  "Warning: componentWillMount is deprecated",
  "Warning: componentWillUpdate is deprecated",
  "Warning: componentWillReceiveProps is deprecated",
  "Module RCTImageLoader requires main queue setup since it overrides `init` but doesn't implement `requiresMainQueueSetup`. In a future release React Native will default to initializing all native modules on a background thread unless explicitly opted-out of.",
  "Warning: Detected multiple renderers concurrently rendering the same context provider. This is currently unsupported.",
])
