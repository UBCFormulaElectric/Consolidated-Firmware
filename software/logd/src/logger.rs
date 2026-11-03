use socketcan::{
    CanFrame,
    Frame,
    EmbeddedFrame,
    StandardId
};

pub fn encode(frame: &CanFrame, buf: &mut Vec<u8>) {
    let mut data = [0u8; 8];
    let src = frame.data();
    let n = src.len().min(8);
    data[..n].clone_from_slice(&src[..n]);

    buf.extend_from_slice(&frame.id_word().to_le_bytes()); // 4 bytes
    buf.extend_from_slice(&data);                          // 8 bytes
}

#[cfg(test)]
mod encode_tests {
    use super::*;

    #[test]
    fn test_encode_basic() {
        let mut buf = Vec::new();
        let id = StandardId::new(0x123).unwrap();
        let frame = CanFrame::new(id, &[1, 2, 3, 4, 5, 6, 7, 8]).unwrap();
        encode(&frame, &mut buf);

        let cmp = vec![
            0x23, 0x01, 0x00, 0x00, 
            1, 2, 3, 4, 5, 6, 7, 8
        ];
        assert_eq!(buf.len(), 12);
        assert_eq!(buf, cmp);
    }
}

pub struct Logger {
    file: std::fs::File,
    buf: Vec<u8>,
}


#[cfg(test)]
mod logger_tests {

}

