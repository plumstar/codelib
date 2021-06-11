import java.nio.charset.StandardCharsets;
import java.util.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * ���ܷ�ʽ����aes-cbc-128 pkcs7����ģʽ
 */
public class AesPkcs7 {

    public static void main(String args[]){
        String data = "���ܵ�����";//���ܵ�����
        String key = "1111111111111111"; //16λ�ַ�����Կ
        String iv = "1111111111111111";//16λ�ַ���ƫ��
        //��������
        String encodeResult = encryptAES(data,key,iv);
        System.out.println(String.format("aes-cbc-128���ܽ�� ->  %s",encodeResult));

        //��������
        String decodeResult = decryptAES(encodeResult,key,iv);
        System.out.println(String.format("���ܽ��: -> %s",decodeResult));

    }

    /**
     * ��������
     * @param data ��������
     * @param key ��Կ
     * @param iv ƫ��
     * @return
     */
    private static String encryptAES(String data,String key,String iv) {
        try {

            Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");
            cipher.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(key.getBytes(StandardCharsets.UTF_8), "AES"), new IvParameterSpec(iv.getBytes(StandardCharsets.UTF_8)));
            byte[] result = cipher.doFinal(pkcs7padding(data, cipher.getBlockSize())); // ������, Ҫ�Լ��ֶ�ʵ�� PKCS7Padding ���
            return Base64.getEncoder().encodeToString(result);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "NULL";
    }

    /**
     * ��������
     * @param data ��������
     * @param key ��Կ
     * @param iv ƫ��
     * @return
     */
    private static String decryptAES(String data, String key,String iv) {
        try {
            Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");
            cipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(key.getBytes(StandardCharsets.UTF_8), "AES"), new IvParameterSpec(iv.getBytes(StandardCharsets.UTF_8)));
            byte[] result = cipher.doFinal(Base64.getDecoder().decode(data));
            return new String(result, StandardCharsets.UTF_8).trim();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "NULL";
    }

    /**
     * pkcs7���
     * @param content
     * @param blockSize
     * @return
     */
    private static byte[] pkcs7padding(String content, int blockSize) {
        byte[] contentBytes = content.getBytes(StandardCharsets.UTF_8);
        int pad = blockSize - (contentBytes.length % blockSize); // ������Ҫ��λ�ĳ���
        byte padChr = (byte) pad; // ��λ�ַ� (���ò�λ����)
        byte[] paddedBytes = new byte[contentBytes.length + pad]; // ��ԭ�еĳ����ϼ��ϲ�λ����
        System.arraycopy(contentBytes, 0, paddedBytes, 0, contentBytes.length); // ԭ�е��ȸ��ƹ�ȥ
        for (int i = contentBytes.length; i < paddedBytes.length; i++) { // ��λ�ַ����
            paddedBytes[i] = padChr;
        }
        return paddedBytes;
    }
}
