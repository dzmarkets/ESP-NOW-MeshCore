//
// File Path: Master/components/security_manager/include/security_manager.h
// Brief:     Header file for security_manager component.
//

#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the pre-configured API Key
 */
const char* security_manager_get_key(void);

/**
 * @brief Encrypts plaintext data using AES-128-CBC
 * @param plaintext The null-terminated input string
 * @param out_buffer The buffer to store the final encrypted payload ([16-byte IV] + [Ciphertext])
 * @param max_len Maximum size of the out_buffer
 * @return The total length of the encrypted payload, or 0 on failure
 */
size_t security_manager_encrypt(const char *plaintext, uint8_t *out_buffer, size_t max_len);

/**
 * @brief Decrypts an incoming AES-128-CBC payload
 * @param payload The raw encrypted incoming bytes ([16-byte IV] + [Ciphertext])
 * @param length The total length of the payload
 * @param out_plaintext The buffer to store the decrypted null-terminated string
 * @param max_len Maximum size of the plaintext buffer
 * @return true if successfully decrypted (Authentication passed), false if bad key/padding
 */
bool security_manager_decrypt(const uint8_t *payload, int length, char *out_plaintext, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // SECURITY_MANAGER_H
