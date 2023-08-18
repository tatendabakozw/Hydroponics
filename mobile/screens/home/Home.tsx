import {
  ScrollView,
  StyleSheet,
  Text,
  TouchableOpacity,
  View,
} from "react-native";
import React, { useEffect, useState } from "react";
import { StatusBar } from "expo-status-bar";
import tw from "twrnc";
import GeneralLayout from "../../layouts/GeneralLayout";
import TempComponent from "../../components/HomeComponents/TempComponent";
import HumidityComponent from "../../components/HomeComponents/HumidityComponent";
import PhComponent from "../../components/HomeComponents/PhComponent";
import DepthComponent from "../../components/HomeComponents/DepthComponent";
import LightComponent from "../../components/HomeComponents/LightComponent";
import socketIOClient from "socket.io-client";
import CurrentWeather from "../../components/Currentweather/CurrentWeather";
import { useNavigation } from "@react-navigation/native";
import HomeHeader from "../../components/Navigation/HomeHeader";
const ENDPOINT = "https://hydroponics-server.onrender.com/";

const socket = socketIOClient(ENDPOINT, {
  transports: ["websocket", "polling", "flashsocket"],
  forceNew: true,
  withCredentials: true,
});

type Props = {};

const Home = (props: Props) => {
  const [all_info, setAllInfo] = useState<any>();
  const navigation = useNavigation();

  useEffect(() => {
    socket.on("values_updates", (data) => {
      setAllInfo(data);
      console.log("got info from backednfr", data);
    });
  }, [socket]);

  return (
    <GeneralLayout>
      <View style={tw`h-full`}>
        <ScrollView contentContainerStyle={tw``}>
          <StatusBar style="auto" />
          <HomeHeader />
          {/* weather info item on top of page */}
          <>
            <CurrentWeather />
          </>
          <Text style={tw`text-2xl text-center text-gray-700 py-8`}>
            Properties
          </Text>
          {/* item for weather info */}
          <View style={tw`flex flex-col justify-between `}>
            <>
              <TempComponent value={all_info?.values?.temp} />
            </>

            <View style={tw`flex flex-row`}>
              <>
                <HumidityComponent value={all_info?.values?.humidity} />
              </>

              <>
                <PhComponent />
              </>
            </View>
            <View style={tw`flex flex-row pt-2`}>
              <>
                <DepthComponent value={all_info?.values?.distance} />
              </>

              <>
                <LightComponent value={all_info?.values?.light} />
              </>
            </View>
            {/* <View style={tw`h-50 bg-red-500 rounded`}>

            </View> */}
          </View>
        </ScrollView>
      </View>
    </GeneralLayout>
  );
};

export default Home;

const styles = StyleSheet.create({});
