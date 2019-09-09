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
    console.log(e)
  }

  if (typeof window !== 'undefined') {
    window.addEventListener('unhandledrejection', e => {
      e.preventDefault(); // 去掉控制台的异常显示
      console.log('unhandledrejection: ', e.reason);
      return true;
    });

    window.addEventListener('rejectionhandled', e => {
      e.preventDefault(); // 去掉控制台的异常显示
      console.log('rejectionhandled: ', e.reason);
      return true;
    });

    window.onerror = function(message, source, lineno, colno, error) {
      // message：错误信息（字符串）。
      // source：发生错误的脚本URL（字符串）
      // lineno：发生错误的行号（数字）
      // colno：发生错误的列号（数字）
      // error：Error对象（对象）
      console.log('捕获到异常: ', { message, source, lineno, colno, error });
      return true; // window.onerror 函数只有在返回 true 的时候, 异常才不会向上抛出, 否则即使是知道异常的发生控制台还是会显示 Uncaught Error: xxxxx
    };

    window.addEventListener('error', (error) => {
      console.log('捕获到异常: ', error);
    }, true)
  }
  else if (typeof process !== 'undefined') {
    process.on('unhandledRejection', e => {
      console.log('unhandledRejection: ', e);
    });

    process.on('uncaughtException', e => {
      console.log('uncaughtException: ', e);
    });
  }
  


  setTimeout(() => {
    Jartto;
});