import React from "react";
import { NavigationContainer } from "@react-navigation/native";
import { createNativeStackNavigator } from "@react-navigation/native-stack";
import Home from "./screens/home/Home";
import Temperature from "./screens/PropertyScreens/Temperature";
import Humidity from "./screens/PropertyScreens/Humidity";
import PH from "./screens/PropertyScreens/PH";
import WaterLevel from "./screens/PropertyScreens/WaterLevel";
import LightIntensity from "./screens/PropertyScreens/LightIntensity";
import Setup from "./screens/setup/Setup";

const Stack = createNativeStackNavigator();

export default function App() {

  return (
    <NavigationContainer>
      <Stack.Navigator>
        <>
        <Stack.Screen
            name="home"
            component={Home}
            options={{ headerShown: false }}
          />
          <Stack.Screen
            name="temperature"
            component={Temperature}
            options={{ headerShown: false }}
          />
          <Stack.Screen
            name="humidity"
            component={Humidity}
            options={{ headerShown: false }}
          />
          <Stack.Screen
            name="ph"
            component={PH}
            options={{ headerShown: false }}
          />
           <Stack.Screen
            name="waterlevel"
            component={WaterLevel}
            options={{ headerShown: false }}
          />
          <Stack.Screen
            name="light"
            component={LightIntensity}
            options={{ headerShown: false }}
          />
           <Stack.Screen
            name="setup"
            component={Setup}
            options={{ headerShown: false }}
          />
        
        </>
      </Stack.Navigator>
    </NavigationContainer>
  );
}
