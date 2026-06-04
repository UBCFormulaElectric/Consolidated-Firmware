"use client";

import { useEffect, useRef } from "react";
import ErrorModal from "../common/ErrorModal";

type SDCardFormatErrorModalProps = {
  errorMessage: string;
  onCancel: () => void;

  options?: {
    label: string;
    onClick: () => void;
  }[];
};

const SDCardFormatErrorModal = (props: SDCardFormatErrorModalProps) => {
  const { errorMessage, options = [], onCancel } = props;

  return (
    <ErrorModal 
      title="Error Formatting SD Card"
      errorMessage={errorMessage}
      onCancel={onCancel}
      options={options}
    />
  );
};

export default SDCardFormatErrorModal;
