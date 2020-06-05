/*
 * amf_non3_gpp_access_registration.h
 *
 *
 */

#ifndef _OpenAPI_amf_non3_gpp_access_registration_H_
#define _OpenAPI_amf_non3_gpp_access_registration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "backup_amf_info.h"
#include "guami.h"
#include "ims_vo_ps.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_non3_gpp_access_registration_s OpenAPI_amf_non3_gpp_access_registration_t;
typedef struct OpenAPI_amf_non3_gpp_access_registration_s {
    char *amf_instance_id;
    char *supported_features;
    int purge_flag;
    char *pei;
    struct OpenAPI_ims_vo_ps_s *ims_vo_ps;
    char *dereg_callback_uri;
    char *amf_service_name_dereg;
    char *pcscf_restoration_callback_uri;
    char *amf_service_name_pcscf_rest;
    struct OpenAPI_guami_s *guami;
    OpenAPI_list_t *backup_amf_info;
    struct OpenAPI_rat_type_s *rat_type;
    int urrp_indicator;
    char *amf_ee_subscription_id;
    char *nid;
    char *registration_time;
    char *vgmlc_address_ipv4;
    char *vgmlc_address_ipv6;
    char *vgmlc_fqdn;
} OpenAPI_amf_non3_gpp_access_registration_t;

OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_create(
    char *amf_instance_id,
    char *supported_features,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_t *ims_vo_ps,
    char *dereg_callback_uri,
    char *amf_service_name_dereg,
    char *pcscf_restoration_callback_uri,
    char *amf_service_name_pcscf_rest,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_rat_type_t *rat_type,
    int urrp_indicator,
    char *amf_ee_subscription_id,
    char *nid,
    char *registration_time,
    char *vgmlc_address_ipv4,
    char *vgmlc_address_ipv6,
    char *vgmlc_fqdn
    );
void OpenAPI_amf_non3_gpp_access_registration_free(OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration);
OpenAPI_amf_non3_gpp_access_registration_t *OpenAPI_amf_non3_gpp_access_registration_parseFromJSON(cJSON *amf_non3_gpp_access_registrationJSON);
cJSON *OpenAPI_amf_non3_gpp_access_registration_convertToJSON(OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_access_registration);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_non3_gpp_access_registration_H_ */
