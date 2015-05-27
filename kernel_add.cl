__kernel void kernel_add(__global float* a)
{
	int gid = get_global_id(0);
	float angle = gid;
	a[gid] = sin(angle) * sin(angle) + 
		cos(angle + 1.0) * cos(angle + 1.0);
}
