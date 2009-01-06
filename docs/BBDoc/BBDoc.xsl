<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:template match="/">
      <html>
         <head>
            <title><xsl:value-of select="docs/title/name" /></title>
            <link rel="Stylesheet" type="text/css" href="BBDoc/BBDoc.css" />
         </head>
         <body>
            <table class="main">
               <xsl:apply-templates select="docs/title" />
               <tr>
                  <xsl:apply-templates select="docs/leftSide" />
                  <xsl:apply-templates select="docs/rightSide" />
               </tr>
            </table>
         </body>
      </html>
   </xsl:template>
   <xsl:template match="/docs/title">
      <tr>
         <td colspan="2" class="invisible">
            <table class="title">
               <tr>
                  <td class="title">
                     <div class="title">
                        <xsl:value-of select="name" />
                     </div>
                     <div class="title version">
                        v<xsl:value-of select="version" />
                     </div>
                  </td>
               </tr>
            </table>
         </td>
      </tr>
   </xsl:template>
   <xsl:template match="/docs/leftSide">
      <td class="invisible">
         <table class="panel">
            <xsl:apply-templates />
         </table>
      </td>
   </xsl:template>
   <xsl:template match="/docs/rightSide">
      <td class="invisible">
         <table class="panel panel2">
            <xsl:apply-templates />
         </table>
      </td>
   </xsl:template>
   <xsl:template match="raised">
      <tr>
         <td class="header">
            <xsl:value-of select="name" />
         </td>
      </tr>
      <tr>
         <td class="info">
            <xsl:apply-templates />
         </td>
      </tr>
   </xsl:template>
   <xsl:template match="flat">
      <tr>
         <td class="header2">
            <table width="100%" bgcolor="#333333">
               <tr>
                  <td>
                     <xsl:value-of select="name" />
                  </td>
               </tr>
            </table>
         </td>
      </tr>
      <tr>
         <td class="info2">
            <xsl:apply-templates />
         </td>
      </tr>
   </xsl:template>
   <xsl:template match="name">
   </xsl:template>
   <xsl:template match="setting/range">
   </xsl:template>
   <xsl:template match="setting/desc">
   </xsl:template>
   <xsl:template match="setting">
      <div class="acks inline">
         <xsl:value-of select="name" />: <xsl:value-of select="range" />
      </div>
      <div class="dark inline">
         &#160;&#160;&#160;<xsl:value-of select="desc" />
      </div>
      <br/>
   </xsl:template>
   <xsl:template match="settings">
      <xsl:apply-templates />
      <p/>
   </xsl:template>
   <xsl:template match="revision/version">
   </xsl:template>
   <xsl:template match="revision/date">
   </xsl:template>
   <xsl:template match="mail">
   </xsl:template>
   <xsl:template match="revision">
      <div class="normal inline">
         <xsl:value-of select="/docs/title/name" />
         v<xsl:value-of select="version" /> (<xsl:value-of select="date" />) /
      </div>
      <xsl:apply-templates />
      <p/>
   </xsl:template>
   <xsl:template match="list">
      <div class="normal">&#8226;<xsl:value-of select="." /></div>
   </xsl:template>
   <xsl:template match="list-dark">
      <div class="dark">&#8226;<xsl:value-of select="." /></div>
   </xsl:template>
   <xsl:template match="text">
      <div class="normal"><xsl:apply-templates /></div>
      <p/>
   </xsl:template>
   <xsl:template match="rightSide/raised/text">
      <div class="acks"><xsl:value-of select="." /></div>
   </xsl:template>
   <xsl:template match="license">
      <div class="version">
         Copyright &#169;
         <xsl:value-of select="date" />&#160;<xsl:value-of select="owner" />
      </div>
      <p/>
      <div class="disclaimer">
         <xsl:value-of select="/docs/title/name"/>
         is provided "as is" without warranty of any kind. The author disclaims
         all warranties, either expressed or implied, including the warranties
         of merchantability and fitness for a particular purpose. In no event
         shall the author or its suppliers be liable for any damages whatsoever
         including direct, indirect, incidental, consequential, loss of business
         profits or special damages, even if the author or its suppliers have
         been advised of the possibility of such damages.
      </div>
   </xsl:template>
   <xsl:template match="quote">
      <div class="quote">
         <xsl:value-of select="." />
      </div>
      <p/>
   </xsl:template>
   <xsl:template match="author">
      <a href="mailto:{../mail}">
         <xsl:value-of select="." />
      </a>
   </xsl:template>
   <xsl:template match="a">
     <a href="{@href}"><xsl:value-of select="." /></a>
   </xsl:template>
   <xsl:template match="div">
     <div class="{@class}"><xsl:value-of select="." /></div>
   </xsl:template>
</xsl:stylesheet>
