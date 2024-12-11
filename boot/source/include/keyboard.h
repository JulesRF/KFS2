/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:04:10 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/03/21 16:23:05 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include "kfs.h"

void    keyboard_init();

extern int     isShiftPressed;
extern int     isCapsPressed;
extern int     isCtrlPressed;

#endif