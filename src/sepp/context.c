/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "context.h"

static sepp_context_t self;

int __sepp_log_domain;

static OGS_POOL(sepp_assoc_pool, sepp_assoc_t);

static int context_initialized = 0;

static int max_num_of_sepp_assoc = 0;

void sepp_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SEPP context */
    memset(&self, 0, sizeof(sepp_context_t));

    ogs_log_install_domain(&__sepp_log_domain, "sepp", ogs_core()->log.level);

#define MAX_NUM_OF_SEPP_ASSOC 8
    max_num_of_sepp_assoc = ogs_app()->max.ue * MAX_NUM_OF_SEPP_ASSOC;

    ogs_pool_init(&sepp_assoc_pool, max_num_of_sepp_assoc);

    context_initialized = 1;
}

void sepp_context_final(void)
{
    ogs_assert(context_initialized == 1);

    sepp_assoc_remove_all();

    ogs_pool_final(&sepp_assoc_pool);

    context_initialized = 0;
}

sepp_context_t *sepp_self(void)
{
    return &self;
}

static int sepp_context_prepare(void)
{
    return OGS_OK;
}

static int sepp_context_validation(void)
{
    return OGS_OK;
}

int sepp_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = sepp_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sepp")) {
            ogs_yaml_iter_t sepp_iter;
            ogs_yaml_iter_recurse(&root_iter, &sepp_iter);
            while (ogs_yaml_iter_next(&sepp_iter)) {
                const char *sepp_key = ogs_yaml_iter_key(&sepp_iter);
                ogs_assert(sepp_key);
                if (!strcmp(sepp_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "discovery")) {
                    /* handle config in sbi library */
                } else if (!strcmp(sepp_key, "info")) {
                    ogs_sbi_nf_instance_t *nf_instance = NULL;
                    ogs_sbi_nf_info_t *nf_info = NULL;
                    ogs_sbi_sepp_info_t *sepp_info = NULL;

                    ogs_yaml_iter_t info_iter;
                    ogs_yaml_iter_recurse(&sepp_iter, &info_iter);

                    nf_instance = ogs_sbi_self()->nf_instance;
                    ogs_assert(nf_instance);

                    nf_info = ogs_sbi_nf_info_add(
                                &nf_instance->nf_info_list,
                                    OpenAPI_nf_type_SEPP);
                    ogs_assert(nf_info);

                    sepp_info = &nf_info->sepp;

                    while (ogs_yaml_iter_next(&info_iter)) {
                        const char *info_key =
                            ogs_yaml_iter_key(&info_iter);
                        ogs_assert(info_key);
                        if (!strcmp(info_key, "domain")) {
                            ogs_yaml_iter_t domain_array, domain_iter;
                            ogs_yaml_iter_recurse(&info_iter, &domain_array);

                            do {
                                if (ogs_yaml_iter_type(&domain_array) ==
                                        YAML_MAPPING_NODE) {
                                    memcpy(&domain_iter, &domain_array,
                                            sizeof(ogs_yaml_iter_t));
                                } else if (ogs_yaml_iter_type(&domain_array) ==
                                            YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(&domain_array))
                                        break;
                                    ogs_yaml_iter_recurse(&domain_array,
                                            &domain_iter);
                                } else if (ogs_yaml_iter_type(&domain_array) ==
                                            YAML_SCALAR_NODE) {
                                    break;
                                } else
                                    ogs_assert_if_reached();

                                while (ogs_yaml_iter_next(&domain_iter)) {
                                    const char *domain_key =
                                        ogs_yaml_iter_key(&domain_iter);
                                    ogs_assert(domain_key);
                                    if (!strcmp(domain_key, "port")) {
                                        ogs_yaml_iter_t port_iter;
                                        ogs_yaml_iter_recurse(&domain_iter,
                                                &port_iter);
                                        while (ogs_yaml_iter_next(&port_iter)) {
                                            const char *port_key =
                                                ogs_yaml_iter_key(&port_iter);
                                            ogs_assert(port_key);
                                            if (!strcmp(port_key, "http")) {
                                                const char *v =
                                                    ogs_yaml_iter_value(
                                                            &port_iter);
                                                if (v) {
                                                    sepp_info->domain[
                                                        sepp_info->
                                                            num_of_domain].
                                                            http.presence =
                                                                true;
                                                    sepp_info->domain[
                                                        sepp_info->
                                                            num_of_domain].
                                                            http.port = atoi(v);
                                                }
                                            } else if (!strcmp(port_key,
                                                        "https")) {
                                                const char *v =
                                                    ogs_yaml_iter_value(
                                                            &port_iter);
                                                if (v) {
                                                    sepp_info->domain[
                                                        sepp_info->
                                                            num_of_domain].
                                                            https.presence =
                                                                true;
                                                    sepp_info->domain[
                                                        sepp_info->
                                                            num_of_domain].
                                                            https.port =
                                                                atoi(v);
                                                }
                                            } else
                                                ogs_warn("unknown key `%s`",
                                                        port_key);
                                        }
                                    } else if (!strcmp(domain_key, "name")) {
                                        sepp_info->domain[
                                            sepp_info->num_of_domain].
                                            name = (char *)ogs_yaml_iter_value(
                                                    &domain_iter);
                                    } else if (!strcmp(domain_key, "fqdn")) {
                                        sepp_info->domain[
                                            sepp_info->num_of_domain].
                                            fqdn = (char *)ogs_yaml_iter_value(
                                                    &domain_iter);
                                    } else
                                        ogs_warn("unknown key `%s`",
                                                domain_key);
                                }

                                if (sepp_info->domain[
                                        sepp_info->num_of_domain].name)
                                    sepp_info->num_of_domain++;

                            } while (ogs_yaml_iter_type(&domain_array) ==
                                    YAML_SEQUENCE_NODE);

                        } else if (!strcmp(info_key, "port")) {
                            ogs_yaml_iter_t port_iter;
                            ogs_yaml_iter_recurse(&info_iter, &port_iter);
                            while (ogs_yaml_iter_next(&port_iter)) {
                                const char *port_key =
                                    ogs_yaml_iter_key(&port_iter);
                                ogs_assert(port_key);
                                if (!strcmp(port_key, "http")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&port_iter);
                                    if (v) {
                                        sepp_info->http.presence = true;
                                        sepp_info->http.port = atoi(v);
                                    }
                                } else if (!strcmp(port_key, "https")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&port_iter);
                                    if (v) {
                                        sepp_info->https.presence = true;
                                        sepp_info->https.port = atoi(v);
                                    }
                                } else
                                    ogs_warn("unknown key `%s`", port_key);
                            }
                        } else
                            ogs_warn("unknown key `%s`", info_key);
                    }
                } else
                    ogs_warn("unknown key `%s`", sepp_key);
            }
        }
    }

    rv = sepp_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

sepp_assoc_t *sepp_assoc_add(ogs_sbi_stream_t *stream)
{
    sepp_assoc_t *assoc = NULL;

    ogs_assert(stream);

    ogs_pool_alloc(&sepp_assoc_pool, &assoc);
    if (!assoc) {
        ogs_error("Maximum number of association[%d] reached",
                    max_num_of_sepp_assoc);
        return NULL;
    }
    memset(assoc, 0, sizeof *assoc);

    assoc->stream = stream;

    ogs_list_add(&self.assoc_list, assoc);

    return assoc;
}

void sepp_assoc_remove(sepp_assoc_t *assoc)
{
    ogs_assert(assoc);

    ogs_list_remove(&self.assoc_list, assoc);

    if (assoc->client)
        ogs_sbi_client_remove(assoc->client);
    if (assoc->nrf_client)
        ogs_sbi_client_remove(assoc->nrf_client);

    ogs_pool_free(&sepp_assoc_pool, assoc);
}

void sepp_assoc_remove_all(void)
{
    sepp_assoc_t *assoc = NULL, *next_assoc = NULL;

    ogs_list_for_each_safe(&self.assoc_list, next_assoc, assoc)
        sepp_assoc_remove(assoc);
}

sepp_assoc_t *sepp_assoc_find(uint32_t index)
{
    return ogs_pool_find(&sepp_assoc_pool, index);
}
