/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:39:34 by sgoldenb          #+#    #+#             */
/*   Updated: 2025/04/30 12:40:46 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Escape sequences for colored output;

#define RESET "\e[0m"
#define BOLD "\e[1m"
#define FAINT "\e[2m"
#define ITALIC "\e[3m"
#define UNDERLINE "\e[4m"
#define BLINK "\e[5m"
#define INVERSE "\e[7m"
#define HIDDEN "\e[8m"
#define STRIKETHROUGH "\e[9m"
#define RESET_BOLD "\e[21m"
#define RESET_FAINT "\e[22m"
#define RESET_ITALIC "\e[23m"
#define RESET_UNDERLINE "\e[24m"
#define RESET_BLINK "\e[25m"
#define RESET_INVERSE "\e[27m"
#define RESET_HIDDEN "\e[28m"
#define RESET_STRIKETHROUGH "\e[29m"

#define BLACK "\e[30m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[34m"
#define MAGENTA "\e[35m"
#define CYAN "\e[36m"
#define WHITE "\e[37m"
#define LIGHT_BLACK "\e[90m"
#define LIGHT_RED "\e[91m"
#define LIGHT_GREEN "\e[92m"
#define LIGHT_YELLOW "\e[93m"
#define LIGHT_BLUE "\e[94m"
#define LIGHT_MAGENTA "\e[95m"
#define LIGHT_CYAN "\e[96m"
#define LIGHT_WHITE "\e[97m"

#define BG_BLACK "\e[40m"
#define BG_RED "\e[41m"
#define BG_GREEN "\e[42m"
#define BG_YELLOW "\e[43m"
#define BG_BLUE "\e[44m"
#define BG_MAGENTA "\e[45m"
#define BG_CYAN "\e[46m"
#define BG_WHITE "\e[47m"
#define BG_LIGHT_BLACK "\e[100m"
#define BG_LIGHT_RED "\e[101m"
#define BG_LIGHT_GREEN "\e[102m"
#define BG_LIGHT_YELLOW "\e[103m"
#define BG_LIGHT_BLUE "\e[104m"
#define BG_LIGHT_MAGENTA "\e[105m"
#define BG_LIGHT_CYAN "\e[106m"
#define BG_LIGHT_WHITE "\e[107m"
