import {
  ScrollView,
  StyleSheet,
  Text,
  TextInput,
  TouchableOpacity,
  View,
} from "react-native";
import React, { useState } from "react";
import GeneralLayout from "../../layouts/GeneralLayout";
import { StatusBar } from "expo-status-bar";
import tw from "twrnc";
import HomeHeader from "../../components/Navigation/HomeHeader";

type Props = {};

const Setup = (props: Props) => {
  const [module_ip, setModuleIp] = useState("");

  return (
    <GeneralLayout>
      <View style={tw`h-full`}>
        <ScrollView contentContainerStyle={tw``}>
          <StatusBar style="auto" />
          <HomeHeader />
          {/* weather info item on top of page */}

          <View style={tw`flex flex-col justify-between `}>
            <ScrollView contentContainerStyle={tw`py-4`}>
              <Text style={tw`text-slate-700 pb-1`}>IP Address:</Text>
              <TextInput
                style={tw`p-4 bg-slate-100 border border-slate-100 rounded-lg`}
                placeholder="Enter Module IP"
                onChange={(text: any) => setModuleIp(text)}
                value={module_ip}
              />
              <TouchableOpacity style={tw`bg-green-700 rounded-lg p-4 my-4`}>
                <Text style={tw`text-white text-center text-lg`}>Set IP</Text>
              </TouchableOpacity>
            </ScrollView>
          </View>
        </ScrollView>
      </View>
    </GeneralLayout>
  );
};

export default Setup;

const styles = StyleSheet.create({});
