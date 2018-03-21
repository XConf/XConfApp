import { navigationOptions } from './config'

export const getHeaderStyleNavigationOptions = style => (
  navigationOptions[style] || navigationOptions.default || {}
)

export const getHeaderOptions = (style) => {
  const options = getHeaderStyleNavigationOptions(style)
  return {
    tintColor: options.headerTintColor,
  }
}

export class CapturedEventsStorage {
  constructor() {
    this._store = {}
  }

  get = (key, type) => {
    if (this._store[key] && this._store[key][type]) {
      const value = this._store[key][type]
      delete this._store[key][type]
      return value
    }

    return null
  };

  set = (key, type, value) => {
    if (!this._store[key]) this._store[key] = {}
    this._store[key][type] = value

    clearTimeout(this._store[key].timeout)
    this._store[key].timeout = setTimeout(() => {
      delete this._store[key]
    }, 10000)
  };
}
