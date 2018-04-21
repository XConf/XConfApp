import React from 'react'
import AppHeader from './Header/AppHeader'

export const navigatorConfig = {
  headerTransitionPreset: 'uikit',
}

export const navigationOptions = {
  default: {
    header: props => <AppHeader {...props} />,
    headerTintColor: '#fff',
  },
  light: {
    headerStyle: {
      backgroundColor: '#fff',
    },
    headerTintColor: '#f4511e',
  },
  'default-nops': {
    header: props => <AppHeader {...props} removePlatformContainerStyles={true} />,
    headerTintColor: '#fff',
  },
}
