
<!--
function resetform()
{
  clrpar();
  setparcrc32();
  document.crcform.data.value="123456789";
}

function clrpar()
{
  document.crcform.order.value="";
  document.crcform.polynom.value="";
  document.crcform.init.value="";
  document.crcform.xor.value="";
  document.crcform.result.value="";
  document.crcform.reflect[0].checked = false;
  document.crcform.reflect[1].checked = false;
  document.crcform.crcinittype[0].checked=true;
  document.crcform.crcinittype[1].checked=false;
  setfocus(document.crcform.order);
}

function setparcrcccitt()
{
  document.crcform.order.value="16";
  document.crcform.polynom.value="1021";
  document.crcform.init.value="FFFF";
  document.crcform.xor.value="0";
  document.crcform.reflect[0].checked = false;
  document.crcform.reflect[1].checked = false;
  document.crcform.crcinittype[0].checked=false;
  document.crcform.crcinittype[1].checked=true;
  setfocus(document.crcform.data);
}

function setparcrc16()
{
  document.crcform.order.value="16";
  document.crcform.polynom.value="8005";
  document.crcform.init.value="0";
  document.crcform.xor.value="0";
  document.crcform.reflect[0].checked = true;
  document.crcform.reflect[1].checked = true;
  document.crcform.crcinittype[0].checked=false;
  document.crcform.crcinittype[1].checked=true;
  setfocus(document.crcform.data);
}

function setparcrc32()
{
  document.crcform.order.value="32";
  document.crcform.polynom.value="4C11DB7";
  document.crcform.init.value="FFFFFFFF";
  document.crcform.xor.value="FFFFFFFF";
  document.crcform.reflect[0].checked = true;
  document.crcform.reflect[1].checked = true;
  document.crcform.crcinittype[0].checked=false;
  document.crcform.crcinittype[1].checked=true;
  setfocus(document.crcform.data);
}

function clrdata()
{
  document.crcform.data.value="";
  setfocus(document.crcform.data);
}


function setfocus(crsr)
{
  crsr.focus();
  crsr.select();
}


function compute()
{
  // computes crc value

  var i;
  var j;
  var k;
  var bit;
  var datalen;
  var len;
  var actchar;
  var flag;
  var counter;
  var c;
  var crc = new Array (8+1);
  var mask = new Array (8);
  var hexnum = new Array ("0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F");

  var data;
  var order;
  var polynom = new Array (8);
  var init = new Array (8);
  var xor = new Array (8);


  // check if all entries are present

  if (document.crcform.order.value == "" || document.crcform.polynom.value == "" ||
      document.crcform.init.value == "" || document.crcform.xor.value == "") 
  {
    document.crcform.result.value= "Invalid parameters";
    return;
  }


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


  // convert crc xor value

  xor = convertentry (document.crcform.xor.value, order);
  if (xor[0]<0)
  {
    document.crcform.result.value = "Invalid XOR value";
    return;
  }


  // generate bit mask

  counter = order;
  for (i=7; i>=0; i--)
  {
    if (counter>=8) mask[i] = 255;
    else mask[i]=(1<<counter)-1;
    counter-=8;
    if (counter<0) counter=0;
  }

  crc = init;

  if (document.crcform.crcinittype[0].checked)		// nondirect -> direct
  {
    crc[8] = 0;

    for (i=0; i<order; i++)
    {
      bit = crc[7-((order-1)>>3)] & (1<<((order-1)&7));
      for (k=0; k<8; k++) {
        crc[k] = ((crc [k] << 1) | (crc [k+1] >> 7)) & mask [k];
        if (bit) crc[k]^= polynom[k];
      }
    }
  }


//  data = unescape(document.crcform.data.value);
  data = document.crcform.data.value;
  datalen = data.length;
  len=0;                     // number of data bytes

  crc[8]=0;


  // main loop, algorithm is fast bit by bit type

  for (i=0; i<datalen; i++)
  {
    c = data.charCodeAt(i);
    if (data.charAt(i)=='%')				// unescape byte by byte (%00 allowed)
    {
      if (i>datalen-3)   
        {
        document.crcform.result.value = "Invalid data sequence";
        return;
        }
      ch = parseInt(data.charAt(++i), 16);
      if (isNaN(ch) == true)
        {
        document.crcform.result.value = "Invalid data sequence";
        return;
        }
      c = parseInt(data.charAt(++i), 16);
      if (isNaN(c) == true)
        {
        document.crcform.result.value = "Invalid data sequence";
        return;
        }
      c = (c&15) | ((ch&15)<<4);
    }


    // perform revin

    if (document.crcform.reflect[0].checked)
      c = reflectByte(c);

    // rotate one data byte including crcmask

    for (j=0; j<8; j++)
    {
      bit=0;
      if (crc[7-((order-1)>>3)] & (1<<((order-1)&7))) bit=1;
      if (c&0x80) bit^=1;
      c<<=1;
      for (k=0; k<8; k++)		// rotate all (max.8) crc bytes
      {
        crc[k] = ((crc [k] << 1) | (crc [k+1] >> 7)) & mask [k];
        if (bit) crc[k]^= polynom[k];
      }
    }
  len++;
  }


  // perform revout

  if (document.crcform.reflect[1].checked)
    crc = reflect(crc, order, 0);


  // perform xor value

  for (i=0; i<8; i++) crc [i] ^= xor [i];


  // write result

  document.crcform.result.value = "";

  flag=0;
  for (i=0; i<8; i++)
  {
    actchar = crc[i]>>4;
    if (flag || actchar)
    {
      document.crcform.result.value+= hexnum[actchar];
      flag=1;
    }

    actchar = crc[i] & 15;
    if (flag || actchar || i==7)
    {
      document.crcform.result.value+= hexnum[actchar];
      flag=1;
    }
  }

  document.crcform.result.value+= " (hex), " + len +" data byte";
  if (len!=1) document.crcform.result.value+= "s";
  setfocus(document.crcform.data);
}




function revpoly ()
{
  // reverses poly

  var polynom = new Array (8);
  var order;
  var actchar;
  var flag;
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

  
  // compute reversed polynom (reflected starting at second LSB that is bit 1)

  polynom = reflect(polynom, order, 1)

  
  // write result

  document.crcform.result.value = "polynom reversed";
  document.crcform.polynom.value = "";

  flag=0;
  for (i=0; i<8; i++)
  {
    actchar = polynom[i]>>4;
    if (flag || actchar)
    {
      document.crcform.polynom.value+= hexnum[actchar];
      flag=1;
    }

    actchar = polynom[i] & 15;
    if (flag || actchar || i==7)
    {
      document.crcform.polynom.value+= hexnum[actchar];
      flag=1;
    }
  }
}

  
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


function reflectByte(inbyte)
{
  // reflect one byte

  var outbyte=0;
  var i=0x01;
  var j;

  for (j=0x80; j; j>>=1) 
  {
    if (inbyte & i) outbyte|=j;
    i<<=1;
  }
  return (outbyte);
}


function reflect(crc, bitnum, startLSB)
{
  // reflect 'bitnum' bits starting at lowest bit = startLSB

  var i, j, k, iw, jw, bit;

  for (k=0; k+startLSB<bitnum-1-k; k++) {

    iw=7-((k+startLSB)>>3);
    jw=1<<((k+startLSB)&7);
    i=7-((bitnum-1-k)>>3);
    j=1<<((bitnum-1-k)&7);

    bit = crc[iw] & jw;
    if (crc[i] & j) crc[iw] |= jw;
    else crc[iw] &= (0xff-jw);
    if (bit) crc[i] |= j;
    else crc[i] &= (0xff-j);
  }

/* old bad code
  iw = 7;                        // LSByte
  jw = 1<<startLSB;              // LSBit, 0=bit0 (crc), 1=bit1 (poly)

  for (i=7-((bitnum-1)>>3); i<8; i++)
  {
    for (j=1<<((bitnum-1)&7); j && (!(i>iw || (i==iw && j<jw))); j>>=1)
    {
      bit = crc[iw] & jw;
      if (crc[i] & j) crc[iw] |= jw;
      else crc[iw] &= (0xff-jw);
      if (bit) crc[i] |= j;
      else crc[i] &= (0xff-j);

      jw = (jw<<1) & 0xff;
      if (!jw) {
        iw--;
        jw=1;
      }
    }
  }
*/
  return(crc);
}


function convertentry (input, order)
{
  // convert from ascii to hexadecimal value (stored as byte sequence)

  var len;
  var actchar;
  var polynom = new Array (0,0,0,0,0,0,0,0);
  var brk = new Array (-1,0,0,0,0,0,0,0);

  // convert crc value into byte sequence

  len = input.length;
  for (i=0; i < len; i++)
  {
    actchar = parseInt(input.charAt(i), 16);
    if (isNaN(actchar) == true) return (brk);
    actchar&=15;

    for (j=0; j<7; j++) polynom[j] = ((polynom [j] << 4) | (polynom [j+1] >> 4)) & 255;
    polynom[7] = ((polynom[7] <<4) | actchar) & 255;
  }


  // compute and check crc order
 
  count = 64;
  for (i=0; i<8; i++)
  {
    for (j=0x80; j; j>>=1)
    {
      if (polynom[i] & j) break;
      count--;
    }
    if (polynom[i] & j) break;
  }

  if (count > order) return (brk);

  return(polynom);
}

//-->
