
float fix_megahertz(int factor,float megahertz) {
   int temp_MHz,temp_mod,temp_div;
   float new_megahertz;
   
   new_megahertz=megahertz;
   temp_MHz=(int)megahertz;
      
   temp_mod=temp_MHz%factor;
   temp_div=temp_MHz/factor;
         
   if (temp_mod<=2) {
      new_megahertz=(float) (temp_div*factor);
   }
   else if (temp_mod>=(factor-2)) {
      new_megahertz=(float) (temp_div+1)*factor;
   }
   
   else new_megahertz=temp_MHz;
   
   return new_megahertz;
}
   
