/* eslint-disable */

import React from 'react'
import PropTypes from 'prop-types'
import { Button } from 'react-native'

import ButtonContext from './ButtonContext'

const AppButton = props => (
  <ButtonContext.Consumer>
    {({
      triggerCapturedPressEvent,
      pressEventCapturingFunction,
      color,
    }) => {
      if (triggerCapturedPressEvent) {
        triggerCapturedPressEvent(props.onPress)
      }

      return (
        <Button
          {...props}
          onPress={pressEventCapturingFunction || props.onPress}
          color={props.color || color}
        />
      )
    }}
  </ButtonContext.Consumer>
)

AppButton.contextTypes = {
  buttonColor: PropTypes.string,
  buttonPressEventCapturingFunction: PropTypes.func,
  capturedButtonPressEvent: PropTypes.array,
}

export default AppButton
