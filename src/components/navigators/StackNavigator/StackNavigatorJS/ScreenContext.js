/* eslint-disable */

import React, { createContext } from 'react'

const RouterContext = createContext()

const Provider = ({ router, children }) => (
  <RouterContext.Provider value={router}>
    {children}
  </RouterContext.Provider>
)

const Consumer = ({ children }) => (
  <RouterContext.Consumer>
    {router => (
      children({ router })
    )}
  </RouterContext.Consumer>
)

const ScreenContext = { Provider, Consumer }

export default ScreenContext
