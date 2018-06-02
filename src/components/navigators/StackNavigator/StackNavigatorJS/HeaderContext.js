/* eslint-disable */

import React, { createContext } from 'react'

const SetNavigationParamsContext = createContext()
const CapturEventContext = createContext()
const TriggerCapturedEventContext = createContext()

const Provider = ({
  setNavigationParams,
  eventCapturingFunction,
  triggerCapturedEvent,
  children,
}) => (
  <SetNavigationParamsContext.Provider value={setNavigationParams}>
    <CapturEventContext.Provider value={eventCapturingFunction}>
      <TriggerCapturedEventContext.Provider value={triggerCapturedEvent}>
        {children}
      </TriggerCapturedEventContext.Provider>
    </CapturEventContext.Provider>
  </SetNavigationParamsContext.Provider>
)

const Consumer = ({ children }) => (
  <SetNavigationParamsContext.Consumer>
    {setNavigationParams => (
      <CapturEventContext.Consumer>
        {eventCapturingFunction => (
          <TriggerCapturedEventContext.Consumer>
            {triggerCapturedEvent => (
              children({
                eventCapturingFunction,
                setNavigationParams,
                triggerCapturedEvent,
              })
            )}
          </TriggerCapturedEventContext.Consumer>
        )}
      </CapturEventContext.Consumer>
    )}
  </SetNavigationParamsContext.Consumer>
)

const ScreenContext = { Provider, Consumer }

export default ScreenContext
