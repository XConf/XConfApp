/* eslint-disable */

import React, { createContext } from 'react'

const RouterContext = createContext()

const Provider = ({ style, children }) => (
  <RouterContext.Provider value={style}>
    {children}
  </RouterContext.Provider>
)

const Consumer = ({ children }) => (
  <RouterContext.Consumer>
    {style => (
      children({ style })
    )}
  </RouterContext.Consumer>
)

const StyleContext = { Provider, Consumer }

export default StyleContext
