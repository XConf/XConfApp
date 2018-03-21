import React from 'react'
import PropTypes from 'prop-types'
import { Button } from 'react-native'

const AppButton = (props, context) => {
  if (context.capturedButtonPressEvent) {
    if (__DEV__) console.disableYellowBox = true
    props.onPress(...context.capturedButtonPressEvent)
    if (__DEV__) console.disableYellowBox = false
  }

  return (
    <Button
      {...props}
      onPress={context.buttonPressEventCapturingFunction || props.onPress}
      color={props.color || context.buttonColor}
    />
  )
}

AppButton.contextTypes = {
  buttonColor: PropTypes.string,
  buttonPressEventCapturingFunction: PropTypes.func,
  capturedButtonPressEvent: PropTypes.array,
}

export default AppButton
