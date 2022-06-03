#ifndef _LINUX_GPS_FIXED_H
#define _LINUX_GPS_FIXED_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/math64.h>

#define M (52)
#define FIXED_PRECISION ((fixed64_t)(1ULL << M))
#define FIXED_PRECISION_D10000 ((fixed64_t)450359962737LL)
#define RAD_PER_DEG ((fixed64_t)0x0000477d1a894a75LL)
#define DEG_PER_RAD ((fixed64_t)0x0394bb834c783f00LL)
#define PRINT_FIXED(x)                                                         \
	do {                                                                   \
		int integer;                                                   \
		u64 fractional;                                                \
		from_fixed(x, &integer, &fractional);                          \
		printk(KERN_DEBUG #x ": %d %lld", integer, fractional);        \
	} while (0);

static __read_mostly const s64 sin_deg[360] = { 0x0LL,
						0x477c2cae2774LL,
						0x8ef2c64fbee1LL,
						0xd65e3a477e48LL,
						0x11db8f6d6a512LL,
						0x164fd6b8c2810LL,
						0x1ac2609b3c577LL,
						0x1f32d44c4f62dLL,
						0x23a0d92d1b3c3LL,
						0x280c16cf50a6fLL,
						0x2c7434fc16e71LL,
						0x30d8dbbaecc4aLL,
						0x3539b35884b1fLL,
						0x3996646d9b8cfLL,
						0x3dee97e5c9724LL,
						0x4241f7064c1a4LL,
						0x46902b74ca38aLL,
						0x4ad8df3e0f572LL,
						0x4f1bbcdcbfa54LL,
						0x53586f4003377LL,
						0x578ea1d2282fdLL,
						0x5bbe007f3b4cfLL,
						0x5fe637bb965a0LL,
						0x6406f48a63febLL,
						0x681fe484186b4LL,
						0x6c30b5dcde615LL,
						0x7039176af8172LL,
						0x7438b8ad13780LL,
						0x782f49d09141eLL,
						0x7c1c7bb7be83bLL,
						0x8000000000000LL,
						0x83d98907eef8aLL,
						0x87a8c9f566e95LL,
						0x8b6d76bb83b8eLL,
						0x8f2744208fe8aLL,
						0x92d5e7c3e24bcLL,
						0x96791823aad2fLL,
						0x9a108ca2adf5bLL,
						0x9d9bfd8dee49cLL,
						0xa11b242243d5eLL,
						0xa48dba91e0b0eLL,
						0xa7f37c09c27eaLL,
						0xab4c24b7105ecLL,
						0xae9771cc64e16LL,
						0xb1d5218703988LL,
						0xb504f333f9de6LL,
						0xb826a7352a69cLL,
						0xbb39ff06434c1LL,
						0xbe3ebd419df61LL,
						0xc134a5a508e1cLL,
						0xc41b7d167a81cLL,
						0xc6f309a8ad194LL,
						0xc9bb129fa31e9LL,
						0xcc73607513d0aLL,
						0xcf1bbcdcbfa54LL,
						0xd1b3f2c8ac3a4LL,
						0xd43bce6d47752LL,
						0xd6b31d45717eeLL,
						0xd919ae166d4a2LL,
						0xdb6f50f3b756fLL,
						0xddb3d742c2655LL,
						0xdfe713be99ce3LL,
						0xe208da7b69384LL,
						0xe41900e9e9636LL,
						0xe6175ddab1c65LL,
						0xe803c9816ebb4LL,
						0xe9de1d77fbfcaLL,
						0xeba634c163338LL,
						0xed5bebccbe5acLL,
						0xeeff2077fdc02LL,
						0xf08fb2129168eLL,
						0xf20d815ff5a65LL,
						0xf378709a22a80LL,
						0xf4d06373ded90LL,
						0xf6153f1af3dc0LL,
						0xf746ea3a45f8aLL,
						0xf8654cfbcdd12LL,
						0xf970510a74388LL,
						0xfa67e193d0040LL,
						0xfb4beb49c5b61LL,
						0xfc1c5c6408e0cLL,
						0xfcd924a17f22eLL,
						0xfd82354984a3dLL,
						0xfe17812d11f46LL,
						0xfe98fca7c33e3LL,
						0xff069da0c0ad1LL,
						0xff605b8b87ffbLL,
						0xffa62f689730eLL,
						0xffd813c5f82b4LL,
						0xfff604bfad7c5LL,
						0x10000000000000LL,
						0xfff604bfad7c5LL,
						0xffd813c5f82b4LL,
						0xffa62f689730eLL,
						0xff605b8b87ffbLL,
						0xff069da0c0ad1LL,
						0xfe98fca7c33e3LL,
						0xfe17812d11f46LL,
						0xfd82354984a3dLL,
						0xfcd924a17f22eLL,
						0xfc1c5c6408e0cLL,
						0xfb4beb49c5b61LL,
						0xfa67e193d0040LL,
						0xf970510a74388LL,
						0xf8654cfbcdd12LL,
						0xf746ea3a45f8aLL,
						0xf6153f1af3dc0LL,
						0xf4d06373ded90LL,
						0xf378709a22a80LL,
						0xf20d815ff5a66LL,
						0xf08fb2129168eLL,
						0xeeff2077fdc02LL,
						0xed5bebccbe5acLL,
						0xeba634c163337LL,
						0xe9de1d77fbfcaLL,
						0xe803c9816ebb4LL,
						0xe6175ddab1c64LL,
						0xe41900e9e9636LL,
						0xe208da7b69384LL,
						0xdfe713be99ce4LL,
						0xddb3d742c2656LL,
						0xdb6f50f3b756fLL,
						0xd919ae166d4a3LL,
						0xd6b31d45717eeLL,
						0xd43bce6d47752LL,
						0xd1b3f2c8ac3a3LL,
						0xcf1bbcdcbfa54LL,
						0xcc73607513d0aLL,
						0xc9bb129fa31e9LL,
						0xc6f309a8ad194LL,
						0xc41b7d167a81cLL,
						0xc134a5a508e1cLL,
						0xbe3ebd419df61LL,
						0xbb39ff06434c2LL,
						0xb826a7352a69cLL,
						0xb504f333f9de6LL,
						0xb1d5218703988LL,
						0xae9771cc64e16LL,
						0xab4c24b7105ecLL,
						0xa7f37c09c27eaLL,
						0xa48dba91e0b0fLL,
						0xa11b242243d5eLL,
						0x9d9bfd8dee49dLL,
						0x9a108ca2adf5aLL,
						0x96791823aad30LL,
						0x92d5e7c3e24bcLL,
						0x8f2744208fe8aLL,
						0x8b6d76bb83b90LL,
						0x87a8c9f566e95LL,
						0x83d98907eef8aLL,
						0x8000000000000LL,
						0x7c1c7bb7be83cLL,
						0x782f49d09141eLL,
						0x7438b8ad13780LL,
						0x7039176af8172LL,
						0x6c30b5dcde615LL,
						0x681fe484186b5LL,
						0x6406f48a63febLL,
						0x5fe637bb965a1LL,
						0x5bbe007f3b4cfLL,
						0x578ea1d2282feLL,
						0x53586f4003376LL,
						0x4f1bbcdcbfa54LL,
						0x4ad8df3e0f571LL,
						0x46902b74ca38aLL,
						0x4241f7064c1a5LL,
						0x3dee97e5c9724LL,
						0x3996646d9b8d0LL,
						0x3539b35884b1fLL,
						0x30d8dbbaecc4bLL,
						0x2c7434fc16e71LL,
						0x280c16cf50a6fLL,
						0x23a0d92d1b3c3LL,
						0x1f32d44c4f62eLL,
						0x1ac2609b3c578LL,
						0x164fd6b8c2810LL,
						0x11db8f6d6a514LL,
						0xd65e3a477e48LL,
						0x8ef2c64fbee2LL,
						0x477c2cae2774LL,
						0x1LL,
						-0x477c2cae2775LL,
						-0x8ef2c64fbee1LL,
						-0xd65e3a477e47LL,
						-0x11db8f6d6a512LL,
						-0x164fd6b8c280fLL,
						-0x1ac2609b3c577LL,
						-0x1f32d44c4f62cLL,
						-0x23a0d92d1b3c4LL,
						-0x280c16cf50a6eLL,
						-0x2c7434fc16e72LL,
						-0x30d8dbbaecc4aLL,
						-0x3539b35884b20LL,
						-0x3996646d9b8ceLL,
						-0x3dee97e5c9723LL,
						-0x4241f7064c1a4LL,
						-0x46902b74ca38aLL,
						-0x4ad8df3e0f572LL,
						-0x4f1bbcdcbfa53LL,
						-0x53586f4003377LL,
						-0x578ea1d2282fdLL,
						-0x5bbe007f3b4d0LL,
						-0x5fe637bb965a0LL,
						-0x6406f48a63feaLL,
						-0x681fe484186b4LL,
						-0x6c30b5dcde614LL,
						-0x7039176af8172LL,
						-0x7438b8ad13780LL,
						-0x782f49d09141eLL,
						-0x7c1c7bb7be83bLL,
						-0x8000000000000LL,
						-0x83d98907eef8aLL,
						-0x87a8c9f566e94LL,
						-0x8b6d76bb83b8eLL,
						-0x8f2744208fe88LL,
						-0x92d5e7c3e24bcLL,
						-0x96791823aad2eLL,
						-0x9a108ca2adf5cLL,
						-0x9d9bfd8dee49cLL,
						-0xa11b242243d60LL,
						-0xa48dba91e0b0eLL,
						-0xa7f37c09c27e9LL,
						-0xab4c24b7105ecLL,
						-0xae9771cc64e15LL,
						-0xb1d5218703988LL,
						-0xb504f333f9de6LL,
						-0xb826a7352a69cLL,
						-0xbb39ff06434c1LL,
						-0xbe3ebd419df62LL,
						-0xc134a5a508e1cLL,
						-0xc41b7d167a81cLL,
						-0xc6f309a8ad192LL,
						-0xc9bb129fa31eaLL,
						-0xcc73607513d0aLL,
						-0xcf1bbcdcbfa54LL,
						-0xd1b3f2c8ac3a2LL,
						-0xd43bce6d47753LL,
						-0xd6b31d45717eeLL,
						-0xd919ae166d4a2LL,
						-0xdb6f50f3b756eLL,
						-0xddb3d742c2654LL,
						-0xdfe713be99ce4LL,
						-0xe208da7b69384LL,
						-0xe41900e9e9636LL,
						-0xe6175ddab1c64LL,
						-0xe803c9816ebb4LL,
						-0xe9de1d77fbfcbLL,
						-0xeba634c163337LL,
						-0xed5bebccbe5acLL,
						-0xeeff2077fdc02LL,
						-0xf08fb2129168eLL,
						-0xf20d815ff5a66LL,
						-0xf378709a22a80LL,
						-0xf4d06373ded8fLL,
						-0xf6153f1af3dc0LL,
						-0xf746ea3a45f8aLL,
						-0xf8654cfbcdd12LL,
						-0xf970510a74387LL,
						-0xfa67e193d0040LL,
						-0xfb4beb49c5b61LL,
						-0xfc1c5c6408e0cLL,
						-0xfcd924a17f22eLL,
						-0xfd82354984a3cLL,
						-0xfe17812d11f46LL,
						-0xfe98fca7c33e4LL,
						-0xff069da0c0ad1LL,
						-0xff605b8b87ffbLL,
						-0xffa62f689730eLL,
						-0xffd813c5f82b4LL,
						-0xfff604bfad7c5LL,
						-0x10000000000000LL,
						-0xfff604bfad7c5LL,
						-0xffd813c5f82b4LL,
						-0xffa62f689730eLL,
						-0xff605b8b87ffcLL,
						-0xff069da0c0ad1LL,
						-0xfe98fca7c33e4LL,
						-0xfe17812d11f46LL,
						-0xfd82354984a3dLL,
						-0xfcd924a17f22eLL,
						-0xfc1c5c6408e0cLL,
						-0xfb4beb49c5b60LL,
						-0xfa67e193d0040LL,
						-0xf970510a74388LL,
						-0xf8654cfbcdd13LL,
						-0xf746ea3a45f8bLL,
						-0xf6153f1af3dc0LL,
						-0xf4d06373ded90LL,
						-0xf378709a22a80LL,
						-0xf20d815ff5a66LL,
						-0xf08fb2129168eLL,
						-0xeeff2077fdc02LL,
						-0xed5bebccbe5acLL,
						-0xeba634c163338LL,
						-0xe9de1d77fbfccLL,
						-0xe803c9816ebb4LL,
						-0xe6175ddab1c65LL,
						-0xe41900e9e9636LL,
						-0xe208da7b69384LL,
						-0xdfe713be99ce2LL,
						-0xddb3d742c2655LL,
						-0xdb6f50f3b756fLL,
						-0xd919ae166d4a4LL,
						-0xd6b31d45717efLL,
						-0xd43bce6d47752LL,
						-0xd1b3f2c8ac3a4LL,
						-0xcf1bbcdcbfa54LL,
						-0xcc73607513d0bLL,
						-0xc9bb129fa31e8LL,
						-0xc6f309a8ad194LL,
						-0xc41b7d167a81dLL,
						-0xc134a5a508e1cLL,
						-0xbe3ebd419df62LL,
						-0xbb39ff06434c0LL,
						-0xb826a7352a69cLL,
						-0xb504f333f9de7LL,
						-0xb1d521870398aLL,
						-0xae9771cc64e14LL,
						-0xab4c24b7105ecLL,
						-0xa7f37c09c27eaLL,
						-0xa48dba91e0b10LL,
						-0xa11b242243d60LL,
						-0x9d9bfd8dee49cLL,
						-0x9a108ca2adf5bLL,
						-0x96791823aad30LL,
						-0x92d5e7c3e24beLL,
						-0x8f2744208fe88LL,
						-0x8b6d76bb83b8eLL,
						-0x87a8c9f566e96LL,
						-0x83d98907eef8bLL,
						-0x8000000000002LL,
						-0x7c1c7bb7be83aLL,
						-0x782f49d09141eLL,
						-0x7438b8ad13781LL,
						-0x7039176af8174LL,
						-0x6c30b5dcde614LL,
						-0x681fe484186b4LL,
						-0x6406f48a63febLL,
						-0x5fe637bb965a2LL,
						-0x5bbe007f3b4d1LL,
						-0x578ea1d2282fdLL,
						-0x53586f4003377LL,
						-0x4f1bbcdcbfa55LL,
						-0x4ad8df3e0f574LL,
						-0x46902b74ca389LL,
						-0x4241f7064c1a4LL,
						-0x3dee97e5c9724LL,
						-0x3996646d9b8d0LL,
						-0x3539b35884b21LL,
						-0x30d8dbbaecc49LL,
						-0x2c7434fc16e71LL,
						-0x280c16cf50a70LL,
						-0x23a0d92d1b3c5LL,
						-0x1f32d44c4f62cLL,
						-0x1ac2609b3c576LL,
						-0x164fd6b8c2811LL,
						-0x11db8f6d6a514LL,
						-0xd65e3a477e4bLL,
						-0x8ef2c64fbee1LL,
						-0x477c2cae2775LL };

typedef s64 fixed64_t;
typedef struct {
	fixed64_t sin0;
	fixed64_t cos0;
} sc0;

static inline int xlround(fixed64_t x)
{
	int q;
	int sign = 1;
	if (x < 0) {
		sign = -1;
		x *= -1;
	}

	q = (int)(x >> (M - 1));
	q = round_up(q, 2);
	q >>= 1;
	return sign * q;
}

// Precondition: x >= y > 0
static inline fixed64_t __xmul(u64 x, u64 y)
{
	u64 s;
	u64 d = 1;
	int i = M;
	s = (y >> i) * x;
	y &= FIXED_PRECISION - 1;
	while (y >= d) {
		s += (y & d) ? (x >> i) : 0;
		d <<= 1;
		--i;
	}
	return (fixed64_t)s;
}

static inline fixed64_t xmul(fixed64_t x, fixed64_t y)
{
	int sign = 1;
	if (!x || !y) {
		return 0;
	}
	if (x < 0) {
		sign *= -1;
		x *= -1;
	}
	if (y < 0) {
		sign *= -1;
		y *= -1;
	}
	return sign *
	       (x >= y ? __xmul((u64)x, (u64)y) : __xmul((u64)y, (u64)x));
}

static inline fixed64_t x2d2(fixed64_t x)
{
	return xmul(x, x) >> 1;
}

static inline fixed64_t x3d6(fixed64_t x, fixed64_t x2d2)
{
	return div_s64(xmul(x, x2d2), 3);
}

static inline fixed64_t __xsin(fixed64_t r, fixed64_t r2d2, fixed64_t r3d6,
			       fixed64_t sin_0, fixed64_t cos_0)
{
	return sin_0 + xmul(r, cos_0) - xmul(r2d2, sin_0) - xmul(r3d6, cos_0);
}

static inline fixed64_t __xcos(fixed64_t r, fixed64_t r2d2, fixed64_t r3d6,
			       fixed64_t sin_0, fixed64_t cos_0)
{
	return cos_0 - xmul(r, sin_0) - xmul(r2d2, cos_0) + xmul(r3d6, sin_0);
}

// Precondition: x_0 \in [0, 360)
static inline sc0 get_sincos0(int x_0)
{
	int x_1 = x_0 <= 90 ? 90 - x_0 : 450 - x_0;
	return (sc0){ sin_deg[x_0], sin_deg[x_1] };
}

static inline fixed64_t to_fixed(s32 integer, s32 fractional)
{
	return (fixed64_t)integer * FIXED_PRECISION +
	       div_s64(((fixed64_t)(fractional)) * FIXED_PRECISION_D10000, 100);
}

static inline void from_fixed(fixed64_t x, s32 *integer, s64 *fractional)
{
	*integer = x >> M;
	*fractional = x - (((s64)(*integer)) << M);
}

static inline fixed64_t xrad(fixed64_t x)
{
	return xmul(x, RAD_PER_DEG);
}

static inline fixed64_t xdeg(fixed64_t x)
{
	return xmul(x, DEG_PER_RAD);
}

typedef fixed64_t (*__fixed_impl_t)(fixed64_t, fixed64_t, fixed64_t, fixed64_t,
				    fixed64_t);

static inline fixed64_t __fixed_call_impl(int x0, fixed64_t d,
					  __fixed_impl_t impl)
{
	sc0 scx0 = get_sincos0(x0);
	fixed64_t r = xrad(d);
	fixed64_t r2d2 = x2d2(r);
	fixed64_t r3d6 = x3d6(r, r2d2);
	return impl(r, r2d2, r3d6, scx0.sin0, scx0.cos0);
}

static inline fixed64_t fixed_call_impl(fixed64_t x, __fixed_impl_t impl)
{
	int x_integer;
	fixed64_t x_fractional;
	x_integer = xlround(x);
	x_fractional = x - (((s64)x_integer) << M);
	if (x_integer < 0) {
		x_integer += 360;
	} else if (x_integer >= 360) {
		x_integer -= 360;
	}
	return __fixed_call_impl(x_integer, x_fractional, impl);
}

static inline fixed64_t xsin(fixed64_t x)
{
	return fixed_call_impl(x, __xsin);
}

static inline fixed64_t xcos(fixed64_t x)
{
	return fixed_call_impl(x, __xcos);
}

fixed64_t cos_to_hav(fixed64_t cos);

fixed64_t calc_hav(const int x_lat_int, const int x_lat_frac,
		   const int x_lng_int, const int x_lng_frac,
		   const int y_lat_int, const int y_lat_frac,
		   const int y_lng_int, const int y_lng_frac);

int is_geo_close(const int x_lat_int, const int x_lat_frac, const int x_lng_int,
		 const int x_lng_frac, const int y_lat_int,
		 const int y_lat_frac, const int y_lng_int,
		 const int y_lng_frac, const int x_acc, const int y_acc);

#endif /* _LINUX_GPS_FIXED_H */
