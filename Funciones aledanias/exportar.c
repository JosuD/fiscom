void escribir(float *z, int n){
	int i;
	FILE *fp;
	fopen(fp, "myfile.txt", "a");
	for(i = 0; i< n, i++){
		fprintf(fp, "%f \n", *(z+i));
	}
	fclose(fp);
}


