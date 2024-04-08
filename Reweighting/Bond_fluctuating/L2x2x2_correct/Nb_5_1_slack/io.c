void read_input_parameters_and_set_default_ones(char *filename){

  FILE *fp;
  char line[200], descriptor[100], value[100];
  int i, n_box_sides;

  // initialise to zero all amplitudes of potentials and flags, too
  A_bond=0;
  A_3=0;
  A_SA=0;
  A_L4=0;

  V_corn_flag=0;
  A_corn=0.0;
  Ncorners=0;
  Nslack=0;
  n_box_sides=0;

  printf("Reading parameters from input file %s\n",filename);
  fp=fopen(filename,"r");

  while (fgets(line, 200, fp)){
    sscanf(line,"%s %s",descriptor,value);

    if (strcmp (descriptor, "Lattice_Dimensionality")==0){
      sscanf(value,"%d",&Lattice_Dimensionality);
      if ((Lattice_Dimensionality!=1) &&
          (Lattice_Dimensionality !=2) &&
          (Lattice_Dimensionality !=3)){
          printf("ERROR. Out of range Lattice_Dimensionality. Admissible values are 1, 2 3.\n");
          exit(0);
        }
    }

    else if (strcmp (descriptor, "L")==0){
      sscanf(value,"%d",&L[n_box_sides]);
      n_box_sides++;
    }

    else if (strcmp (descriptor, "Nbonds")==0){
      sscanf(value,"%d",&Nbonds);
      if (fabs(A_bond) < EPS) { // parameter is still unspecified, set it to default value
        A_bond=(1.0); // equal to 1 in 2021 PRL, checked
      }
    }
   
    else if (strcmp (descriptor, "Nslack")==0){
      sscanf(value,"%d",&Nslack);
    }

    else if (strcmp (descriptor, "V_SA_flag")==0){
      sscanf(value,"%d",&V_SA_flag);
      if (fabs(A_SA) < EPS) { // parameter is still unspecified, set it to default value
       A_SA=(V_SA_flag*120.0); // equal to 5!=120 for consistency with 2021 PRL, checked
      }

      // XXX in the future we might want to have a separate flag for V3. Now it is enslaved to V_SA
      if (fabs(A_3) < EPS) { // parameter is still unspecified, set it to default value
        A_3=(V_SA_flag*6.0); // equal to 3!=6 in 2021 PRL, checked
      }
    }

    else if (strcmp (descriptor, "V_L4_flag")==0){
      sscanf(value,"%d",&V_L4_flag);
      if (fabs(A_L4) < EPS) { // parameter is still unspecified, set it to default value
        A_L4=(V_L4_flag*24.0); // equal to 4!=24 in 2021 PRL, checked
      }
    }

    else if (strcmp (descriptor, "Ncorners")==0){
      sscanf(value,"%lf",&Ncorners);
      V_corn_flag=1;
      if (fabs(A_corn) < EPS) { // parameter is still unspecified, set it to default value
        A_corn=(V_corn_flag);
      }
    }

    // Specifying amplitudes is optional. But if they are specified, teh default values are overridden

    else if (strcmp (descriptor, "A_bond")==0){
      sscanf(value,"%lf",&A_bond);
    }

    else if (strcmp (descriptor, "A_3")==0){
      sscanf(value,"%lf",&A_3);
    }

    else if (strcmp (descriptor, "A_SA")==0){
      sscanf(value,"%lf",&A_SA);
    }

    else if (strcmp (descriptor, "A_L4")==0){
      sscanf(value,"%lf",&A_L4);
    }

    else if (strcmp (descriptor, "A_corn")==0){
      sscanf(value,"%lf",&A_corn);
    }

    else {
      printf("Error. Unknown descriptor: %s\n",descriptor);
      exit(1);
    }

  }
  fclose(fp);

  /* Check that the number of specified box sides matches the declared dimensionality */

   if (Lattice_Dimensionality!= n_box_sides){
    printf("Error. Number of specified lattice sides does not match declared lattice dimensionality\n");
    exit(1);
   }



  printf("Finished reading parameters.\n\n");
  printf("Summary of parameters:\n");
  printf("\tLattice_Dimensionality: %d\n",Lattice_Dimensionality);
  printf("\tL: "); for (i=0; i < Lattice_Dimensionality; i++) printf(" %d",L[i]); printf("\n");
  printf("\tNbonds: %d\tA_bond: %lf\t\n",Nbonds,A_bond);
  printf("\tV_SA_flag: %d\tA_SA: %lf\tA_3: %lf\n",V_SA_flag,A_SA,A_3);
  printf("\tV_L4_flag: %d\tA_L4: %lf\n\n",V_L4_flag,A_L4);
  printf("\tV_corn_flag: %d\tNcorners: %2.1lf\n",V_corn_flag,Ncorners);
  printf("\tNumber of slack variables: %d\n", Nslack);
  printf("\n\n");

}

/****************************************/


void  write_out_fields_and_couplings(struct Tensors_and_spin_array system){

  int i,j, QUBO_fields_not_null;
  FILE *fp;
  char filename[500], box_volume[50];

  // write in a string the volume of the box
  if (Lattice_Dimensionality==1) sprintf(box_volume,"%d",L[0]);
  else if (Lattice_Dimensionality==2) sprintf(box_volume,"%dx%d",L[0],L[1]);
  else if (Lattice_Dimensionality==3) sprintf(box_volume,"%dx%dx%d",L[0],L[1],L[2]);
  else{
    printf("Error in writing out the volume of the box. This should not have happened.\n");
    exit(1);
  }

  // First write out results in QUBO formalism
  if (V_corn_flag == 1) {
    if (Nslack == 0) {
      sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners);
    }
    else  {
      sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf_ns%d.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners, Nslack);
    }
  }
  else if (V_corn_flag == 0)  sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d.dat",box_volume, Nbonds, V_SA_flag,V_L4_flag);


  fp=fopen(filename,"w");

  // Constant term to the energy
  fprintf(fp,"# additive constant\n");
  fprintf(fp,"%lf\n",system.QUBO_offset);
  // Number of total qubits and corresponding values
  fprintf(fp,"# %4d local fields\n",system.n_spins);
  for(i=0; i < system.n_spins; i++){
    fprintf(fp,"%d %lf\n",i,system.QUBO_field[i]);
  }
  // Couplings
  QUBO_fields_not_null = 0;
  fprintf(fp,"# non-zero couplings\n");
  for(i=0; i < system.n_spins; i++){
    for(j=0; j < system.n_spins; j++){
      if(fabs(system.QUBO_coupling[i][j])>1.0e-6) {
        fprintf(fp,"%d %d %lf\n",i,j,system.QUBO_coupling[i][j]);
        QUBO_fields_not_null += 1;
      }
    }
  }
  fclose(fp);

  printf("Written QUBO fields and couplings to file: %s\n",filename);

  // First write out results in QUBO formalism
  if (V_corn_flag == 1) {
    if (Nslack == 0) {
      sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf.qubo",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners);
    }
    else {
      sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf_ns%d.qubo",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners,Nslack);
    }
  }
  else if (V_corn_flag == 0)  sprintf(filename,"QUBO_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d.qubo",box_volume, Nbonds, V_SA_flag,V_L4_flag);

  fp=fopen(filename,"w");

  // We write a comment with the energy target of the groundstate
  fprintf(fp,"c Energy offset %lf\n", system.QUBO_offset);
  // First row with number of spins of problem and number of non-zero couplings
  fprintf(fp,"c qubo topology maxNodes nNodes nCouplers\n");
  fprintf(fp,"p qubo 0 %d %d %d\n", system.n_spins, system.n_spins, QUBO_fields_not_null/2);
  // Fields
  fprintf(fp, "c Fields\n");
  for(i=0; i < system.n_spins; i++){
    fprintf(fp,"%d %d %lf\n",i,i, system.QUBO_field[i]);
  }
  // Couplings
  fprintf(fp,"c Couplings\n");
  for(i=0; i < system.n_spins-1; i++){
    for(j=i+1; j < system.n_spins; j++){
      if (fabs(system.QUBO_coupling[i][j])>1.0e-6) fprintf(fp,"%d %d %lf\n",i,j,2.0*system.QUBO_coupling[i][j]);
    }
  }
  fclose(fp);

  printf("Written QUBO fields and couplings to file: %s\n",filename);

  // Now write out results in Ising formalism
  if (V_corn_flag == 1) {
    if (Nslack == 0) {  
      sprintf(filename,"Ising_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners);
    }
    else {
      sprintf(filename,"Ising_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf_ns%d.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners,Nslack);
    }
  }
  else if (V_corn_flag == 0)  sprintf(filename,"Ising_fields_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d.dat",box_volume, Nbonds, V_SA_flag,V_L4_flag);

  fp=fopen(filename,"w");

  // Constant term to the energy
  fprintf(fp,"# additive constant\n");
  fprintf(fp,"%lf\n",system.Ising_offset);
  // Number of total qubits and corresponding values
  fprintf(fp,"# %4d local fields\n",system.n_spins);
  for(i=0; i < system.n_spins; i++){
    fprintf(fp,"%d %lf\n",i,system.Ising_field[i]);
  }
  // Couplings
  fprintf(fp,"# non-zero couplings\n");
  for(i=0; i < system.n_spins; i++){
    for(j=0; j < system.n_spins; j++){
      if (fabs(system.Ising_coupling[i][j])>1.0e-6) fprintf(fp,"%d %d %lf\n",i,j,system.Ising_coupling[i][j]);
    }
  }
  fclose(fp);

  printf("Written Ising fields and couplings to file: %s\n",filename);

  // Now write out offset to sum to Dwave
  if (V_corn_flag == 1) {
    if (Nslack == 0)  {  
      sprintf(filename,"Dwave_offset_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners);
    } else { 
      sprintf(filename,"Dwave_offset_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d_nc%2.1lf_ns%d.dat",box_volume, Nbonds,V_SA_flag,V_L4_flag,Ncorners,Nslack);
    }
  }
  else if (V_corn_flag == 0)  sprintf(filename,"Dwave_offset_and_couplings_L%s_nb%d_V_SAflag%d_V_L4flag%d.dat",box_volume, Nbonds, V_SA_flag,V_L4_flag);

  fp=fopen(filename,"w");

  // Write down the Offset
  fprintf(fp, "%lf", system.Dwave_offset);

  fclose(fp);

  printf("Written Dwave_offset: %s\n", filename);

  // Write down the files for the input for Dwave
  // Field file
  fp=fopen("h.txt", "w");

  for(i=0; i < system.n_spins; i++){
    fprintf(fp,"%lf\n",system.Ising_field[i]);
  }

  fclose(fp);

  // Coupling file
  fp=fopen("J.txt", "w");

  for(i=0; i < system.n_spins; i++){
    for(j=0; j < system.n_spins; j++){
      fprintf(fp,"%lf ",system.Ising_coupling[i][j]);
    }
    fprintf(fp,"\n");
  }

  fclose(fp);
}
