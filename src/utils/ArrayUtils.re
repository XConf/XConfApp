let safeGet = (array, index) =>
  if (index >= Array.length(array)) {
    None;
  } else {
    Some(array[index]);
  };
