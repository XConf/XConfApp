/* eslint-disable */

import React, { createContext } from 'react'

const TriggerCapturedPressEventContext = createContext()
const PressEventCapturingFunctionContext = createContext()
const ColorContext = createContext()

const Provider = ({
  triggerCapturedPressEvent,
  pressEventCapturingFunction,
  color,
  children,
}) => (
  <TriggerCapturedPressEventContext.Provider value={triggerCapturedPressEvent}>
    <PressEventCapturingFunctionContext.Provider value={pressEventCapturingFunction}>
      <ColorContext.Provider value={color}>
        {children}
      </ColorContext.Provider>
    </PressEventCapturingFunctionContext.Provider>
  </TriggerCapturedPressEventContext.Provider>
)

const Consumer = ({ children }) => (
  <TriggerCapturedPressEventContext.Consumer>
    {triggerCapturedPressEvent => (
      <PressEventCapturingFunctionContext.Consumer>
        {pressEventCapturingFunction => (
          <ColorContext.Consumer>
            {color => (
              children({
                triggerCapturedPressEvent,
                pressEventCapturingFunction,
                color,
              })
            )}
          </ColorContext.Consumer>
        )}
      </PressEventCapturingFunctionContext.Consumer>
    )}
  </TriggerCapturedPressEventContext.Consumer>
)

const ButtonContext = { Provider, Consumer }

export default ButtonContext
