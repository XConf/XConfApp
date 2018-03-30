/**
 * Proxies JS objects to make them directly accessable by BuckleScript,
 * saving memory and gives better performance when updating changes back.
 *
 * See the navigators in this app for examples.
 */

/* eslint-disable prefer-destructuring */

export class BsProxy {
  constructor() {
    this.cachedProxyHandlers = {}
    this.proxied = this.proxied.bind(this)
  }

  proxied(opt = {}, jsObject) {
    const options = typeof opt === 'function' ? opt() : opt

    // option
    if (options.optional) {
      // None
      if (jsObject == null) return 0
    }

    const { type: jsType, as: bsType } = options

    let bsObject

    switch (jsType) {
      case 'object':
        bsObject = this.proxyObjectForRecord(options.properties, jsObject)
        break
      case 'array':
        switch (bsType) {
          case 'list':
            bsObject = this.proxyArrayForList(options.options, jsObject)
        }
        break
      default:
        bsObject = jsObject
        break
    }

    // ref
    if (options.reference) {
      bsObject = [bsObject]
    }

    // option
    if (options.optional) {
      // Some
      bsObject = [bsObject]
    }

    return bsObject
  }

  proxyObjectForRecord(properties, jsObject) {
    if (this.cachedProxyHandlers[properties]) {
      const proxyHandler = this.cachedProxyHandlers[properties]
      return new Proxy(jsObject, proxyHandler)
    }

    const self = this
    const proxyHandler = {
      get(obj, prop) {
        if (prop === '__bs_proxy__') return true
        if (prop === '__bs_type__') return 'record'
        if (prop === '__raw__') return obj

        switch (prop) {
          case 'toJSON':
            return obj.toJSON
          case 'slice': {
            // Clone the object
            const clonedBsRecord = properties.map((property) => {
              return self.getObjValue(obj, property)
            })
            return () => clonedBsRecord
          }
        }

        const property = properties[prop]
        if (!property) return undefined
        return self.getObjValue(obj, property)
      },
    }
    this.cachedProxyHandlers[properties] = proxyHandler

    return new Proxy(jsObject, proxyHandler)
  }

  getObjValue(obj, property) {
    const { name, resolver, value: options } = property
    const value = typeof resolver === 'function' ? resolver(obj) : obj[name]
    return this.proxied(options, value)
  }

  proxyArrayForList(options, jsArray) {
    if (jsArray.length === 0) return 0

    if (this.cachedProxyHandlers[options]) {
      const proxyHandler = this.cachedProxyHandlers[options]
      return new Proxy({ array: jsArray }, proxyHandler)
    }

    const self = this
    const proxyHandler = {
      get({ array, offset = 0 }, prop) {
        if (prop === '__bs_proxy__') return true
        if (prop === '__bs_type__') return 'list'
        if (prop === '__raw__') return { array, offset }

        switch (prop) {
          case 0:
          case '0': {
            const index = options.reverse ? array.length - offset - 1 : offset
            const value = array[index]
            const { value: valueOptions } = options
            return self.proxied(valueOptions, value)
          }
          case 1:
          case '1':
            if (offset >= array.length - 1) return 0
            return new Proxy({ array, offset: offset + 1 }, proxyHandler)
          case 'length':
            return 2
          default:
            return undefined
        }
      },
    }
    this.cachedProxyHandlers[options] = proxyHandler

    const { startIndex = 0 } = options

    return new Proxy({ array: jsArray, offset: startIndex }, proxyHandler)
  }
}

export class BsProxyUpdate {
  constructor() {
    this.updated = this.updated.bind(this)
  }

  updated(opt = {}, originalObject, bsObject) {
    const options = typeof opt === 'function' ? opt() : opt
    const { type: jsType, as: bsType } = options

    if (typeof bsObject === 'object' && bsObject.__bs_proxy__) {
      switch (bsObject.__bs_type__) {
        case 'record':
          return bsObject.__raw__
        case 'list':
          return bsObject.__raw__.array
      }
    }

    let bsValue = bsObject

    // ref
    if (options.reference) {
      bsValue = bsValue[0]
    }

    // option
    if (options.optional) {
      // None
      if (bsValue === 0) return undefined
      // Some
      bsValue = bsValue[0]
    }

    switch (bsType) {
      case 'record':
        return this.recordToObjectUpdator(options.properties, originalObject, bsValue)
      case 'list':
        return this.listToArrayUpdator(options.options, originalObject, bsValue)
      default:
        return bsValue
    }
  }

  recordToObjectUpdator(properties, originalObject, record) {
    const newObject = Object.assign({}, originalObject)

    properties.forEach((property, i) => {
      const {
        name,
        value: options,
        resolver,
        updator,
      } = property

      const originalValue = originalObject &&
        (typeof resolver === 'function' ? resolver(originalObject) : originalObject[name])

      const newValue = this.updated(options, originalValue, record[i])
      if (typeof updator === 'function') {
        updator(newObject, newValue)
      } else {
        newObject[name] = newValue
      }
    })

    return newObject
  }

  listToArrayUpdator(options, originalArray, list) {
    if (list === 0) {
      if (originalArray && originalArray.length === 0) return originalArray
      return []
    }
    let newArray = []

    let i = list

    const { value: valueOptions, reverse } = options

    while (i) {
      if (i.__bs_proxy__) {
        if (reverse) {
          newArray = i.__raw__.array.concat(newArray)
        } else {
          newArray = newArray.concat(i.__raw__.array)
        }

        break
      }

      const v = this.updated(valueOptions, undefined, i[0])

      if (reverse) {
        newArray = [v].concat(newArray)
      } else {
        newArray.push(v)
      }
      i = i[1]
    }

    return newArray
  }
}

export default BsProxy
