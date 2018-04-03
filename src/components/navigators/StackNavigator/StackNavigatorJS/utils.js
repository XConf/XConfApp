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
    this._triggers = {}
  }

  // get = (key, type, trigger) => {
  //   if (this._store[key] && this._store[key][type]) {
  //     const value = this._store[key][type]

  //     if (value) {
  //       delete this._store[key][type]
  //       return value
  //     }

  //     if (trigger) {
  //       if (!this._triggers[key]) this._triggers[key] = {}
  //       this._triggers[key][type] = trigger

  //       clearTimeout(this._triggers[key].timeout)
  //       this._triggers[key].timeout = setTimeout(() => {
  //         delete this._triggers[key]
  //       }, 10000)
  //     }
  //   }

  //   return null
  // };

  set = (key, type, value) => {
    if (this._triggers[key] && this._triggers[key][type]) {
      this._triggers[key][type](value)
      return
    }

    if (!this._store[key]) this._store[key] = {}
    this._store[key][type] = value

    clearTimeout(this._store[key].timeout)
    this._store[key].timeout = setTimeout(() => {
      delete this._store[key]
    }, 10000)
  };


  trigger = (key, type, func) => {
    if (typeof func !== 'function') return

    if (this._store[key] && this._store[key][type]) {
      const value = this._store[key][type]

      if (value) {
        delete this._store[key][type]
        func(value)
      }

      if (!this._triggers[key]) this._triggers[key] = {}
      this._triggers[key][type] = func

      clearTimeout(this._triggers[key].timeout)
      this._triggers[key].timeout = setTimeout(() => {
        delete this._triggers[key]
      }, 10000)
    }
  }
}
