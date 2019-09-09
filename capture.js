// 如果浏览支持Promise，捕获promise里面then的报错，因为promise里面的错误onerror和try-catch都无法捕获
if (Promise && Promise.prototype.then) {
  var promiseThen = Promise.prototype.then;
  
  Promise.prototype.then = function(resolve, reject) {
    return promiseThen.call(this, wrapPromiseFunction(resolve), wrapPromiseFunction(reject));
  }
}

/**
 * 输入一个函数，将函数内代码包裹进try-catch执行，then的resolve、reject和普通函数不一样
 * 
 * @param {any} fn 
 * @returns 一个新的函数
 */
function wrapPromiseFunction(fn) {

  // 如果fn是个函数，则直接放到try-catch中运行，否则要将类的方法包裹起来，promise中的fn要返回null，不能返回空函数
  if (typeof fn !== 'function') {
    return null;
  }

  return function () {
    try {
      return fn.apply(this, arguments);
    }
    catch (e) {
      errorProcess(e);
      throw e;
    }
  };
}

function errorProcess(e) {

}

if (window) {
  window.onerror = function (msg, url, line) {
    // 可以捕获异步函数中的错误信息并进行处理，提示Script error.
    console.log(msg);   // 获取错误信息
    console.log(url);   // 获取出错的文件路径
    console.log(line);  // 获取错误出错的行数
  };
}

/**
 * 封装React方法的错误处理,改成使用入参的prototype中是否有render生命周期函数来判断
 * @param  {object} Component 传入组件
 * @return {object} 返回包裹后的组件
 */
function defineReact(Component) {

  var proto = Component.prototype;
  var key;

  // 封装本身constructor中的构造方法，React组件编译为ES5后是一个构造函数，ES6下面为class
  if (_isTrueFunction(Component)) {
    Component = wrapFunction(Component);
  }

  var componnetKeys = Object.getOwnPropertyNames(proto);

  // 支持ES6类的属性方法错误捕获
  for (var i = 0, len = componnetKeys.length; i < len; i++) {
    key = componnetKeys[i];
    proto[key] = wrapFunction(proto[key]);
  }

  // 支持ES5下的属性方法错误捕获
  for (key in proto) {
    if (typeof proto[key] === 'function') {
      proto[key] = wrapFunction(proto[key]);
    }
  }

  return Component;
}

/**
* 判断是否为真实的函数，而不是class
* @param  {Function} fn [description]
* @return {Boolean}     [description]
*/
function _isTrueFunction(fn) {

  var isTrueFunction = false;

  try {
    isTrueFunction = fn.prototype.constructor.arguments === null;
  }
  catch (e) {
    isTrueFunction = false;
  }

  for (var key in fn.prototype) {
    return false;
  }

  return isTrueFunction;
}
