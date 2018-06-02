/* eslint-disable */

import React, { createContext } from 'react'

const ContentContext = createContext()

const Provider = ({ content, children }) => (
  <ContentContext.Provider value={content}>
    {children}
  </ContentContext.Provider>
)

const Consumer = ({ children }) => (
  <ContentContext.Consumer>
    {content => (
      children({ content })
    )}
  </ContentContext.Consumer>
)

const BaseContentContext = { Provider, Consumer }

export default BaseContentContext
