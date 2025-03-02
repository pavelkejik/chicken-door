export function pick(obj, keys) {
    return keys.reduce((result, key) => {
      if (obj && obj.hasOwnProperty(key)) {
        result[key] = obj[key];
      }
      return result;
    }, {});
  }
  