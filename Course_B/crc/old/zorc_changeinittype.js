function chgtype ()
{
  // change crc init type (algorithm dependend)

  var init = new Array (8+1);
  var order;
  var actchar;
  var flag;
  var mask = new Array (8);
  var polynom = new Array (8);
  var hexnum = new Array ("0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F");


  document.crcform.result.value = "";

  // convert crc order

  order=parseInt(document.crcform.order.value, 10);
  if (isNaN(order) == true || order<1 || order>64)
  {
    document.crcform.result.value = "CRC order must be between 1 and 64";
    return;
  }


  // convert crc polynom

  polynom = convertentry (document.crcform.polynom.value, order);
  if (polynom[0]<0)
  {
    document.crcform.result.value = "Invalid CRC polynom";
    return;
  }

  // check if polynom is valid (bit 0 must be set)
  
//  if (!(polynom[7]&1))
//  {
//    document.crcform.result.value = "CRC polynom LSB must be set";
//    return;
//  }

  

  // convert crc init value

  init = convertentry (document.crcform.init.value, order);
  if (init[0]<0)
  {
    document.crcform.result.value = "Invalid initial value";
    return;
  }


  // algorithm starts here (bit by bit type)
  // generate and byte mask

  counter = order;
  for (i=7; i>=0; i--)
  {
    if (counter>=8) mask[i] = 255;
    else mask[i]=(1<<counter)-1;
    counter-=8;
    if (counter<0) counter=0;
  }

  
  // compute new init value

  if (document.crcform.crcinittype[0].checked)		// nondirect -> direct
  {
    init[8] = 0;

    for (i=0; i<order; i++) {

      bit = init[7-((order-1)>>3)] & (1<<((order-1)&7));
      for (k=0; k<8; k++) {
        init[k] = ((init [k] << 1) | (init [k+1] >> 7)) & mask [k];
        if (bit) init[k]^= polynom[k];
      }
    }	
    document.crcform.crcinittype[0].checked=false;
    document.crcform.crcinittype[1].checked=true;
    document.crcform.result.value = "converted to direct initial value";
  }

  else
  {
    for (i=0; i<order; i++) {				// direct -> nondirect

      bit=init[7]&1;
      for (k=0; k<8; k++) if (bit) init[k]^= polynom[k];
      for (k=7; k; k--) init[k] = ((init [k] >> 1) | ((init[k-1]&1) << 7)) & mask [k];
      init[0]>>=1;
      if (bit) init[7-((order-1)>>3)] |= 1<<((order-1)&7);

    }	
    document.crcform.crcinittype[0].checked=true;
    document.crcform.crcinittype[1].checked=false;
    document.crcform.result.value = "converted to nondirect initial value";
  }


  
  // write result

  document.crcform.init.value = "";

  flag=0;
  for (i=0; i<8; i++)
  {
    actchar = init[i]>>4;
    if (flag || actchar)
    {
      document.crcform.init.value+= hexnum[actchar];
      flag=1;
    }

    actchar = init[i] & 15;
    if (flag || actchar || i==7)
    {
      document.crcform.init.value+= hexnum[actchar];
      flag=1;
    }
  }
}
