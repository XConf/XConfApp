import React from 'react'
import PropTypes from 'prop-types'
import { Button } from 'react-native'

const AppButton = (props, context) => (
  <Button
    {...props}
    color={props.color || context.buttonColor}
  />
)

AppButton.contextTypes = {
  buttonColor: PropTypes.string,
}

export default AppButton
