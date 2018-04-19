/* eslint-disable global-require */
if (typeof Proxy === 'undefined') {
  const ProxyPolyfill = require('proxy-polyfill/src/proxy')
  global.PolyfilledProxy = new ProxyPolyfill()
}
