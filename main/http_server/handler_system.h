#pragma once

esp_err_t GET_system_info(httpd_req_t *req);
esp_err_t PATCH_update_settings(httpd_req_t *req);

esp_err_t GET_system_asic(httpd_req_t *req);

esp_err_t GET_ethernet_status(httpd_req_t *req);
esp_err_t POST_ethernet_config(httpd_req_t *req);
esp_err_t POST_network_mode(httpd_req_t *req);