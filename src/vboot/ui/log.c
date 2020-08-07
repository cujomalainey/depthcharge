// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2020 Google Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <vb2_api.h>

#include "vboot/ui.h"

vb2_error_t ui_log_init(struct ui_log_info *log, const char *str)
{
	uint32_t lines_per_page, chars_per_line;
	uint32_t chars_current_line;
	uint32_t lines;
	uint32_t pages;
	const char *ptr;

	VB2_TRY(ui_get_log_textbox_dimensions(&lines_per_page,
					      &chars_per_line));

	if (lines_per_page == 0 || chars_per_line == 0 || str == NULL) {
		UI_ERROR("Failed to initialize log_info, "
			 "lines_per_page: %u, chars_per_line: %u, "
			 "str: %s\n",
			 lines_per_page, chars_per_line, str ? str : "NULL");
		return VB2_ERROR_UI_LOG_INIT;
	}

	free(log->page_start);
	memset(log, 0, sizeof(*log));

	/* Count number of characters and lines. */
	lines = 0;
	ptr = str;
	while (*ptr != '\0') {
		chars_current_line = 0;
		lines++;
		while (*ptr != '\0') {
			chars_current_line++;
			if (*ptr == '\n') {
				ptr++;
				break;
			}
			/* Wrap current line, put current character into next
			   line. */
			if (chars_current_line > chars_per_line)
				break;
			ptr++;
		}
	}

	/* Initialize log_info entries. */
	log->str = str;
	log->page_count = DIV_ROUND_UP(lines, lines_per_page);
	log->page_start = malloc((log->page_count + 1) * sizeof(const char *));
	if (!log->page_start) {
		UI_ERROR("Failed to malloc page_start array, "
			 "page_count: %u, log_string: %s\n",
			 log->page_count, log->str);
		return VB2_ERROR_UI_MEMORY_ALLOC;
	}

	/* Calculate page_start. */
	lines = 0;
	pages = 0;
	ptr = str;
	while (*ptr != '\0') {
		if (lines % lines_per_page == 0)
			log->page_start[pages++] = ptr;
		chars_current_line = 0;
		lines++;
		while (*ptr != '\0') {
			chars_current_line++;
			if (*ptr == '\n') {
				ptr++;
				break;
			}
			/* Wrap current line, put current character into next
			   line. */
			if (chars_current_line > chars_per_line)
				break;
			ptr++;
		}
	}
	/* Set the last entry to the end of log string. */
	log->page_start[pages] = ptr;

	UI_INFO("Initialize log_info with page_count: %u, "
		"lines_per_page: %u, chars_per_line: %u\n",
		log->page_count, lines_per_page, chars_per_line);

	return VB2_SUCCESS;
}

char *ui_log_get_page_content(const struct ui_log_info *log, uint32_t page)
{
	uint32_t lines_per_page, chars_per_line;
	int i;
	char *buf;
	uint32_t chars_current_line;
	const char *ptr, *line_start;

	if (ui_get_log_textbox_dimensions(&lines_per_page, &chars_per_line)) {
		UI_ERROR("Failed to get log textbox dimensions, "
			 "lines_per_page: %u, chars_per_line: %u\n",
			 lines_per_page, chars_per_line);
		return NULL;
	}

	if (page >= log->page_count) {
		UI_ERROR("Failed to get page content, "
			 "page: %u, page_count: %u\n", page, log->page_count);
		return NULL;
	}

	buf = malloc((chars_per_line + 1) * lines_per_page + 1);
	if (!buf) {
		UI_ERROR("Failed to malloc string buffer, page: %u, "
			 "lines_per_page: %u, chars_per_line: %u\n",
			 page, lines_per_page, chars_per_line);
		return NULL;
	}

	i = 0;
	ptr = log->page_start[page];
	while (ptr < log->page_start[page + 1]) {
		chars_current_line = 0;
		line_start = ptr;
		while (ptr <= log->page_start[page + 1]) {
			chars_current_line++;
			if (*ptr == '\n' || *ptr == '\0') {
				strncpy(buf + i, line_start,
					chars_current_line);
				i += chars_current_line;
				if (*ptr == '\n')
					ptr++;
				break;
			}
			/* Wrap current line, put current character into next
			   line. */
			if (chars_current_line > chars_per_line) {
				strncpy(buf + i, line_start, chars_per_line);
				i += chars_per_line;
				buf[i++] = '\n';  /* Add newline manually. */
				break;
			}
			ptr++;
		}
	}
	/* No newline at the end of the last line. */
	if (i > 0 && buf[i - 1] == '\n')
		i--;
	buf[i] = '\0';

	UI_INFO("Get page content from log_info with page: %u\n", page);

	return buf;
}