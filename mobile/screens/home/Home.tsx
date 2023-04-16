import { ScrollView, StyleSheet, Text, View } from "react-native";
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
const ENDPOINT = "https://hydroponics-server.onrender.com/";

const socket = socketIOClient(ENDPOINT, {
  transports: ["websocket", "polling", "flashsocket"],
  forceNew: true,
  withCredentials: true,
});

type Props = {};

const Home = (props: Props) => {
  const [all_info, setAllInfo] = useState<any>();

  useEffect(() => {
    socket.on("values_updates", (data) => {
      setAllInfo(data);
      console.log("got info from backednfr", data);
    });
  }, [socket]);

  return (
    <GeneralLayout>
      <View style={tw`h-full`}>
        <ScrollView contentContainerStyle={tw``} >
          <StatusBar style="auto" />
          <View style={tw`flex flex-col w-full bg-white pb-4`}>
            <View
              style={tw`flex flex-row items-center p-1 bg-gray-100 rounded`}
            >
              <View style={tw`bg-green-700 px-2 py-3 rounded flex-1`}>
                <Text style={tw`text-white text-xl font-semibold text-center`}>
                  Overview
                </Text>
              </View>
              <View style={tw`px-2 rounded flex-1`}>
                <Text
                  style={tw`text-gray-500 text-center text-xl font-semibold`}
                >
                  Notes
                </Text>
              </View>
            </View>
          </View>
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
