/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schai <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 11:27:47 by schai             #+#    #+#             */
/*   Updated: 2022/11/14 16:40:27 by schai            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>

typedef unsigned long long	t_ull;

int		ft_printf(const char *format, ...);
int		check_nb(const char *format, va_list arg);
int		check_format(const char *format, va_list arg);
void	ft_putstr(char *str);
int		ft_strlenint(char *str);
int		ft_print_c(va_list arg);
int		ft_print_s(va_list arg);
int		ft_print_di(va_list arg);
int		ft_plen(uintptr_t n);
void	ft_putp(uintptr_t n);
int		ft_print_p(va_list arg);
int		ft_len_u(unsigned int n);
char	*ft_itoa_u(unsigned int n);
int		ft_print_u(va_list arg);
int		ft_len_hex(unsigned int n);
void	ft_put_hex(unsigned int n);
void	ft_put_hex_maj(unsigned int n);
int		ft_print_x_maj(va_list arg);
int		ft_print_x(va_list arg);

#endif
