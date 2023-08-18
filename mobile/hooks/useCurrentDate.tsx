import axios from "axios";
import { useEffect, useRef, useReducer, useContext, useCallback } from "react";
import { getError } from "../utils/error";

export const useCurrentDate = () => {
    const date = new Date();
    let day = date.getDate();
    let month = date.getMonth() + 1;
    let year = date.getFullYear();
  
    // This arrangement can be altered based on how we want the date's format to appear.
    let currentDate = `${day}-${month}-${year}`;


  return currentDate ;
};
