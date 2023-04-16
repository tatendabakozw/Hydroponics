import React from "react";
import { ReactNode } from "react";
import { StyleSheet, View, ScrollView, SafeAreaView, Text } from "react-native";
import tw from "twrnc";
import CustomHeader from "../components/Navigation/CustomHeader";

type Props = {
  children?: ReactNode;
};

const GeneralLayout = ({ children }: Props) => {
  return (
    <SafeAreaView style={{ flex: 1 }}>
      <View>
        <CustomHeader />
      </View>
      <ScrollView
        style={[tw`flex-1 bg-white h-full`, { paddingHorizontal: 10 }]}
      >
        {children}
      </ScrollView>
    </SafeAreaView>
  );
};

export default GeneralLayout;
