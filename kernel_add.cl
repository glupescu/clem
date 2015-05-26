__kernel void kernel_add(__global int* a)
{
	int gid = get_global_id(0);
	a[gid] = gid;
}
