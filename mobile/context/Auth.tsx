import React, { createContext, useState, useContext, useEffect } from "react";
import AsyncStorage from "@react-native-async-storage/async-storage";

import { AuthData, authService } from "../services/authService";
import axios from "axios";
import { apiUrl } from "../utils/apiUrl";

type AuthContextData = {
  authData?: AuthData;
  loading: boolean;
  signIn: any;
  signOut(): void;
};

//Create the Auth Context with the data type specified
//and a empty object
const AuthContext = createContext<AuthContextData>({} as AuthContextData);

//@ts-ignore
const AuthProvider: React.FC = ({ children }) => {
  const [authData, setAuthData] = useState<AuthData>();

  //the AuthContext start with loading equals true
  //and stay like this, until the data be load from Async Storage
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    //Every time the App is opened, this provider is rendered
    //and call de loadStorage function.
    loadStorageData();
  }, []);

  async function loadStorageData(): Promise<void> {
    try {
      //Try get the data from Async Storage
      const authDataSerialized = await AsyncStorage.getItem("meeta_user_info");
      if (authDataSerialized) {
        //If there are data, it's converted to an Object and the state is updated.
        const _authData: AuthData = JSON.parse(authDataSerialized);
        setAuthData(_authData);
      }
    } catch (error) {
    } finally {
      //loading finished
      setLoading(false);
    }
  }

  const signIn = async (email: string, _password: string) => {
    //call the service passing credential (email and password).
    //In a real App this data will be provided by the user from some InputText components.
    try {
      const { data } = await axios.post(`${apiUrl}/auth/login`, {
        username: "",
        password: "",
      });
      //Set the data in the context, so the App can be notified
      //and send the user to the AuthStack
      setAuthData(data);
      //Persist the data in the Async Storage
      //to be recovered in the next user session.
      AsyncStorage.setItem("meeta_user_info", JSON.stringify(data));
    } catch (error) {
      console.log(error);
    }
  };

  const signOut = async () => {
    //Remove data from context, so the App can be notified
    //and send the user to the AuthStack
    setAuthData(undefined);

    //Remove the data from Async Storage
    //to NOT be recoverede in next session.
    await AsyncStorage.removeItem("meeta_user_info");
  };

  return (
    //This component will be used to encapsulate the whole App,
    //so all components will have access to the Context
    <AuthContext.Provider value={{ authData, loading, signIn, signOut }}>
      {children}
    </AuthContext.Provider>
  );
};

//A simple hooks to facilitate the access to the AuthContext
// and permit components to subscribe to AuthContext updates
function useAuth(): AuthContextData {
  const context = useContext(AuthContext);

  if (!context) {
    throw new Error("useAuth must be used within an AuthProvider");
  }

  return context;
}

export { AuthContext, AuthProvider, useAuth };
