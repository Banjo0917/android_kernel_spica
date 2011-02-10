/* linux/arch/arm/plat-s3c/gpio-config.c
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008-2010 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C series GPIO configuration core
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/io.h>

#include <plat/gpio-core.h>
#include <plat/gpio-cfg.h>
#include <plat/gpio-cfg-helpers.h>

#include <mach/map.h>
#include <mach/regs-gpio.h>

int s3c_gpio_cfgpin(unsigned int pin, unsigned int config)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	unsigned long flags;
	int offset;
	int ret;

	if (!chip)
		return -EINVAL;

	offset = pin - chip->chip.base;

	s3c_gpio_lock(chip, flags);
	ret = s3c_gpio_do_setcfg(chip, offset, config);
	s3c_gpio_unlock(chip, flags);

	return ret;
}
EXPORT_SYMBOL(s3c_gpio_cfgpin);

unsigned s3c_gpio_getcfg(unsigned int pin)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	unsigned long flags;
	unsigned ret = 0;
	int offset;

	if (chip) {
		offset = pin - chip->chip.base;

		s3c_gpio_lock(chip, flags);
		ret = s3c_gpio_do_getcfg(chip, offset);
		s3c_gpio_unlock(chip, flags);
	}

	return ret;
}
EXPORT_SYMBOL(s3c_gpio_getcfg);


int s3c_gpio_setpull(unsigned int pin, s3c_gpio_pull_t pull)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	unsigned long flags;
	int offset, ret;

	if (!chip)
		return -EINVAL;

	offset = pin - chip->chip.base;

	s3c_gpio_lock(chip, flags);
	ret = s3c_gpio_do_setpull(chip, offset, pull);
	s3c_gpio_unlock(chip, flags);

	return ret;
}
EXPORT_SYMBOL(s3c_gpio_setpull);

int s3c_gpio_slp_cfgpin(unsigned int pin, unsigned int config)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	void __iomem *reg;
	unsigned long flags;
	int offset;
	u32 con;
	int shift;

	if (!chip)
		return -EINVAL;
	if((chip->base == (S3C64XX_GPK_BASE + 0x4)) ||
		(chip->base == (S3C64XX_GPL_BASE + 0x4)) ||
		(chip->base == S3C64XX_GPM_BASE) ||
		(chip->base == S3C64XX_GPN_BASE))
	{
		return -EINVAL;
	}

	if(config > 3)
	{
			return -EINVAL;
	}

	reg = chip->base + 0x0C;

	offset = pin - chip->chip.base;
	shift = offset * 2;

	local_irq_save(flags);

	con = __raw_readl(reg);
	con &= ~(3 << shift);
	con |= config << shift;
		__raw_writel(con, reg);

	local_irq_restore(flags);

	return 0;
}
EXPORT_SYMBOL(s3c_gpio_slp_cfgpin);

s3c_gpio_pull_t s3c_gpio_get_slp_cfgpin(unsigned int pin)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	void __iomem *reg;
	unsigned long flags;
	int offset;
	u32 con;
	int shift;

	if (!chip)
		return -EINVAL;
	if((chip->base == (S3C64XX_GPK_BASE + 0x4)) ||
		(chip->base == (S3C64XX_GPL_BASE + 0x4)) ||
		(chip->base == S3C64XX_GPM_BASE) ||
		(chip->base == S3C64XX_GPN_BASE))
	{
		return -EINVAL;
	}

	reg = chip->base + 0x0C;

	offset = pin - chip->chip.base;
	shift = offset * 2;

	local_irq_save(flags);

	con = __raw_readl(reg);
	con >>= shift;
	con &= 0x3;

	local_irq_restore(flags);

	return (__force s3c_gpio_pull_t)con;
}
EXPORT_SYMBOL(s3c_gpio_get_slp_cfgpin);

int s3c_gpio_slp_setpull_updown(unsigned int pin, unsigned int config)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	void __iomem *reg;
	unsigned long flags;
	int offset;
	u32 con;
	int shift;

	if (!chip)
		return -EINVAL;
	if((chip->base == (S3C64XX_GPK_BASE + 0x4)) ||
		(chip->base == (S3C64XX_GPL_BASE + 0x4)) ||
		(chip->base == S3C64XX_GPM_BASE) ||
		(chip->base == S3C64XX_GPN_BASE))
	{
		return -EINVAL;
	}
	if(config > 3)
	{
		return -EINVAL;
	}
	reg = chip->base + 0x10;

	offset = pin - chip->chip.base;
	shift = offset * 2;

	local_irq_save(flags);

	con = __raw_readl(reg);
	con &= ~(3 << shift);
	con |= config << shift;
	__raw_writel(con, reg);

	con = __raw_readl(reg);

	local_irq_restore(flags);

	return 0;
}
EXPORT_SYMBOL(s3c_gpio_slp_setpull_updown);

s3c_gpio_pull_t s3c_gpio_slp_getpull_updown(unsigned int pin)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	void __iomem *reg;
	unsigned long flags;
	int offset;
	u32 con;
	int shift;

	if (!chip)
		return -EINVAL;
	if((chip->base == (S3C64XX_GPK_BASE + 0x4)) ||
		(chip->base == (S3C64XX_GPL_BASE + 0x4)) ||
		(chip->base == S3C64XX_GPM_BASE) ||
		(chip->base == S3C64XX_GPN_BASE))
	{
		return -EINVAL;
	}

	reg = chip->base + 0x10;

	offset = pin - chip->chip.base;
	shift = offset * 2;

	local_irq_save(flags);

	con = __raw_readl(reg);
	con >>= shift;
	con &= 0x3;

	local_irq_restore(flags);

	return (__force s3c_gpio_pull_t)con;
}
EXPORT_SYMBOL(s3c_gpio_slp_getpull_updown);

#ifdef CONFIG_S3C_GPIO_CFG_S3C24XX
int s3c_gpio_setcfg_s3c24xx_a(struct s3c_gpio_chip *chip,
			      unsigned int off, unsigned int cfg)
{
	void __iomem *reg = chip->base;
	unsigned int shift = off;
	u32 con;

	if (s3c_gpio_is_cfg_special(cfg)) {
		cfg &= 0xf;

		/* Map output to 0, and SFN2 to 1 */
		cfg -= 1;
		if (cfg > 1)
			return -EINVAL;

		cfg <<= shift;
	}

	con = __raw_readl(reg);
	con &= ~(0x1 << shift);
	con |= cfg;
	__raw_writel(con, reg);

	return 0;
}

unsigned s3c_gpio_getcfg_s3c24xx_a(struct s3c_gpio_chip *chip,
				   unsigned int off)
{
	u32 con;

	con = __raw_readl(chip->base);
	con >>= off;
	con &= 1;
	con++;

	return S3C_GPIO_SFN(con);
}

int s3c_gpio_setcfg_s3c24xx(struct s3c_gpio_chip *chip,
			    unsigned int off, unsigned int cfg)
{
	void __iomem *reg = chip->base;
	unsigned int shift = off * 2;
	u32 con;

	if (s3c_gpio_is_cfg_special(cfg)) {
		cfg &= 0xf;
		if (cfg > 3)
			return -EINVAL;

		cfg <<= shift;
	}

	con = __raw_readl(reg);
	con &= ~(0x3 << shift);
	con |= cfg;
	__raw_writel(con, reg);

	return 0;
}

unsigned int s3c_gpio_getcfg_s3c24xx(struct s3c_gpio_chip *chip,
				     unsigned int off)
{
	u32 con;

	con = __raw_readl(chip->base);
	con >>= off * 2;
	con &= 3;

	/* this conversion works for IN and OUT as well as special mode */
	return S3C_GPIO_SPECIAL(con);
}
#endif

#ifdef CONFIG_S3C_GPIO_CFG_S3C64XX
int s3c_gpio_setcfg_s3c64xx_4bit(struct s3c_gpio_chip *chip,
				 unsigned int off, unsigned int cfg)
{
	void __iomem *reg = chip->base;
	unsigned int shift = (off & 7) * 4;
	u32 con;

	if (off < 8 && chip->chip.ngpio > 8)
		reg -= 4;

	if (s3c_gpio_is_cfg_special(cfg)) {
		cfg &= 0xf;
		cfg <<= shift;
	}

	con = __raw_readl(reg);
	con &= ~(0xf << shift);
	con |= cfg;
	__raw_writel(con, reg);

	return 0;
}

unsigned s3c_gpio_getcfg_s3c64xx_4bit(struct s3c_gpio_chip *chip,
				      unsigned int off)
{
	void __iomem *reg = chip->base;
	unsigned int shift = (off & 7) * 4;
	u32 con;

	if (off < 8 && chip->chip.ngpio > 8)
		reg -= 4;

	con = __raw_readl(reg);
	con >>= shift;
	con &= 0xf;

	/* this conversion works for IN and OUT as well as special mode */
	return S3C_GPIO_SPECIAL(con);
}

#endif /* CONFIG_S3C_GPIO_CFG_S3C64XX */

#ifdef CONFIG_S3C_GPIO_PULL_UPDOWN
int s3c_gpio_setpull_updown(struct s3c_gpio_chip *chip,
			    unsigned int off, s3c_gpio_pull_t pull)
{
	void __iomem *reg = chip->base + 0x08;
	int shift = off * 2;
	u32 pup;

	pup = __raw_readl(reg);
	pup &= ~(3 << shift);
	pup |= pull << shift;
	__raw_writel(pup, reg);

	return 0;
}

s3c_gpio_pull_t s3c_gpio_getpull_updown(struct s3c_gpio_chip *chip,
					unsigned int off)
{
	void __iomem *reg = chip->base + 0x08;
	int shift = off * 2;
	u32 pup = __raw_readl(reg);

	pup >>= shift;
	pup &= 0x3;
	return (__force s3c_gpio_pull_t)pup;
}
#endif

#ifdef CONFIG_S3C_GPIO_PULL_UP
int s3c_gpio_setpull_1up(struct s3c_gpio_chip *chip,
			 unsigned int off, s3c_gpio_pull_t pull)
{
	void __iomem *reg = chip->base + 0x08;
	u32 pup = __raw_readl(reg);

	pup = __raw_readl(reg);

	if (pup == S3C_GPIO_PULL_UP)
		pup &= ~(1 << off);
	else if (pup == S3C_GPIO_PULL_NONE)
		pup |= (1 << off);
	else
		return -EINVAL;

	__raw_writel(pup, reg);
	return 0;
}

s3c_gpio_pull_t s3c_gpio_getpull_1up(struct s3c_gpio_chip *chip,
				     unsigned int off)
{
	void __iomem *reg = chip->base + 0x08;
	u32 pup = __raw_readl(reg);

	pup &= (1 << off);
	return pup ? S3C_GPIO_PULL_NONE : S3C_GPIO_PULL_UP;
}
#endif /* CONFIG_S3C_GPIO_PULL_UP */

#ifdef CONFIG_S5P_GPIO_DRVSTR
s5p_gpio_drvstr_t s5p_gpio_get_drvstr(unsigned int pin)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	unsigned int off;
	void __iomem *reg;
	int shift;
	u32 drvstr;

	if (!chip)
		return -EINVAL;

	off = chip->chip.base - pin;
	shift = off * 2;
	reg = chip->base + 0x0C;

	drvstr = __raw_readl(reg);
	drvstr = 0xffff & (0x3 << shift);
	drvstr = drvstr >> shift;

	return (__force s5p_gpio_drvstr_t)drvstr;
}
EXPORT_SYMBOL(s5p_gpio_get_drvstr);

int s5p_gpio_set_drvstr(unsigned int pin, s5p_gpio_drvstr_t drvstr)
{
	struct s3c_gpio_chip *chip = s3c_gpiolib_getchip(pin);
	unsigned int off;
	void __iomem *reg;
	int shift;
	u32 tmp;

	if (!chip)
		return -EINVAL;

	off = chip->chip.base - pin;
	shift = off * 2;
	reg = chip->base + 0x0C;

	tmp = __raw_readl(reg);
	tmp |= drvstr << shift;

	__raw_writel(tmp, reg);

	return 0;
}
EXPORT_SYMBOL(s5p_gpio_set_drvstr);
#endif	/* CONFIG_S5P_GPIO_DRVSTR */
