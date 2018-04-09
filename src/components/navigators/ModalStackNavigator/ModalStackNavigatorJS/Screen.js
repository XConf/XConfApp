import React from 'react'

import ScreenContext from './ScreenContext'

const Screen = ({ navigation: { state: { params: { route } } } }) => (
  <ScreenContext.Consumer>
    {({ router }) => router(route)}
  </ScreenContext.Consumer>
)

export default Screen
